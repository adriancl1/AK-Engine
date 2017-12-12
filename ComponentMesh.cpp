#include "ComponentMesh.h"
#include "Application.h"
#include "ComponentTransform.h"
#include "ResourceMesh.h"
#include "GameObject.h"
#include "Primitive.h"
#include "Configuration.h"
#include "ModuleImporter.h"

#include "imgui-1.51/imgui.h"
#include "Glew/include/glew.h"
#include "MathGeo/Geometry/Triangle.h"
#include "MathGeo/Math/float4x4.h"
#include "MathGeo/Geometry/LineSegment.h"

ComponentMesh::ComponentMesh() : Component(Component_Mesh)
{
	name = "Mesh";
}

ComponentMesh::~ComponentMesh()
{
	if (mesh != nullptr)
	{
		mesh->UnloadFromComponent();
	}
}

void ComponentMesh::Update()
{
	ResetDeformable();
}

float3 ComponentMesh::GetCenter() const
{
	if (mesh != nullptr)
	{
		return 	mesh->enclosingBox.Centroid();
	}
}

void ComponentMesh::DrawDebug() const
{
	if (mesh->idNormals > 0)
	{
		for (int i = 0; i < mesh->numVertices * 3; i += 3)
		{
			pLine vNormal(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2], mesh->vertices[i] + mesh->normals[i], mesh->vertices[i + 1] + mesh->normals[i + 1], mesh->vertices[i + 2] + mesh->normals[i + 2]);
			vNormal.color = Green;
			vNormal.Render();
		}
		if (mesh->numVertices % 3 == 0)
		{
			for (int i = 0; i < mesh->numIndices; i += 3)
			{

				Triangle face(float3(mesh->vertices[mesh->indices[i] * 3], mesh->vertices[mesh->indices[i] * 3 + 1], mesh->vertices[mesh->indices[i] * 3 + 2]), float3(mesh->vertices[mesh->indices[i + 1] * 3], mesh->vertices[mesh->indices[i + 1] * 3 + 1], mesh->vertices[mesh->indices[i + 1] * 3 + 2]), float3(mesh->vertices[mesh->indices[i + 2] * 3], mesh->vertices[mesh->indices[i + 2] * 3 + 1], mesh->vertices[mesh->indices[i + 2] * 3 + 2]));
				float3 faceCenter = face.Centroid();
				float3 faceNormal = face.NormalCCW();
				pLine normal(faceCenter.x, faceCenter.y, faceCenter.z, faceCenter.x + faceNormal.x, faceCenter.y + faceNormal.y, faceCenter.z + faceNormal.z);
				normal.color = Orange;
				normal.Render();
			}
		}
	}

	//Draw enclosing box ----
	float3 corners[8];
	mesh->enclosingBox.GetCornerPoints(corners);

	glColor3f(Azure.r, Azure.g, Azure.b);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);

	//TODO: improve this function
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy, sz);
	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy, sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx,  sy, sz);
	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy, sz);

	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx,  sy, -sz);

	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f(sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f(sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f(sx,  sy, -sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f(sx,  sy,  sz);

	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy,  sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);

	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx, sy,  sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx, sy,  sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx, sy, -sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx, sy, -sz);

	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);

	glEnd();

	glColor3f(1, 1, 1);
}

void ComponentMesh::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		if (mesh != nullptr)
		{
			ImGui::Text("Vertices ID: %i", mesh->idVertices);
			ImGui::Text("Num Vertices: %i", mesh->numVertices);
			ImGui::Text("Indexes ID: %i", mesh->idIndices);
			ImGui::Text("Num Indexes: %i", mesh->numIndices);
			ImGui::Text("Normals ID: %i", mesh->idNormals);
			ImGui::Text("Colors ID: %i", mesh->idColors);
			ImGui::Text("Texture Coords: %i", mesh->idTexCoords);
			ImGui::Text("Resource ID: %i", mesh->GetUID());
			ImGui::Text("Resource reference counting: %i", mesh->GetReferenceCount());
		}
		else
		{
			ImGui::Text("Missing resource mesh!");
		}
		if (ImGui::Button("Delete Component"))
		{
			wantsToDie = true;
		}
		ImGui::TreePop();
	}
}

bool ComponentMesh::IntersectsAABB(LineSegment & line) const
{
	LineSegment globalLine(line);
	ComponentTransform* myGOTransform = (ComponentTransform*)myGO->FindComponent(Component_Transform);

	globalLine.Transform(myGOTransform->GetGlobalTransform().Inverted());
	return globalLine.Intersects(mesh->enclosingBox);
}

bool ComponentMesh::TriIntersection(LineSegment & line, float& distance, float3 &hitPoint)
{
	bool ret = false;

	float minDistance = distance;
	float prevDistance = distance;

	LineSegment localLine(line);

	ComponentTransform* myGOTransform = (ComponentTransform*)myGO->FindComponent(Component_Transform);

	localLine.Transform(myGOTransform->GetGlobalTransform().Inverted());

	for (int i = 0; i < mesh->numIndices; i += 3)
	{
		Triangle face(float3(mesh->vertices[mesh->indices[i] * 3], mesh->vertices[mesh->indices[i] * 3 + 1], mesh->vertices[mesh->indices[i] * 3 + 2]), float3(mesh->vertices[mesh->indices[i + 1] * 3], mesh->vertices[mesh->indices[i + 1] * 3 + 1], mesh->vertices[mesh->indices[i + 1] * 3 + 2]), float3(mesh->vertices[mesh->indices[i + 2] * 3], mesh->vertices[mesh->indices[i + 2] * 3 + 1], mesh->vertices[mesh->indices[i + 2] * 3 + 2]));
		if (localLine.Intersects(face, &minDistance, &hitPoint))
		{
			if (minDistance < prevDistance)
			{
				prevDistance = minDistance;
				distance = minDistance;
				ret = true;
			}
		}
	}
	return ret;
}

void ComponentMesh::OnSave(Configuration & data) const
{
	if (mesh != nullptr)
	{
		data.SetInt("Resource UID", mesh->GetUID());
	}
}

void ComponentMesh::OnLoad(Configuration & data)
{
	AddResource(data.GetInt("Resource UID"));
}

void ComponentMesh::AddResource(int UID)
{
	mesh = (ResourceMesh*)App->resources->Get(UID);
	mesh->LoadToComponent();
}

uint ComponentMesh::GetIDVertices() const
{
	if (mesh != nullptr)
	{
		return mesh->idVertices;
	}
	return 0;
}

uint ComponentMesh::GetNumVertices() const
{
	if (mesh != nullptr)
	{
		return mesh->numVertices;
	}
	return 0;
}

uint ComponentMesh::GetIDIndices() const
{
	if (mesh != nullptr)
	{
		return mesh->idIndices;
	}
	return 0;
}

uint ComponentMesh::GetNumIndices() const
{
	if (mesh != nullptr)
	{
		return mesh->numIndices;
	}
	return 0;
}

uint ComponentMesh::GetIDNormals() const
{
	if (mesh != nullptr)
	{
		return mesh->idNormals;
	}
	return 0;
}

uint ComponentMesh::GetIDColors() const
{
	if (mesh != nullptr)
	{
		return mesh->idColors;
	}
	return 0;
}

uint ComponentMesh::GetIDTextCoords() const
{
	if (mesh != nullptr)
	{
		return mesh->idTexCoords;
	}
	return 0;
}

AABB ComponentMesh::GetEnclosingBox() const
{
	if (mesh != nullptr)
	{
		return mesh->enclosingBox;
	}
	return AABB(float3(0, 0, 0), float3(0, 0, 0));
}

void ComponentMesh::createDeformable()
{
	meshDeformable = new Mesh();
	meshDeformable->vertices = new float[mesh->numVertices * 3 * sizeof(float)];
	memcpy(meshDeformable->vertices, mesh->vertices, mesh->numVertices * 3 * sizeof(float));
	meshDeformable->numVertices = mesh->numVertices;
	meshDeformable->normals = new float[mesh->numVertices * 3 * sizeof(float)];
	memcpy(meshDeformable->normals, mesh->normals, mesh->numVertices * 3 * sizeof(float));

	glGenBuffers(1, (GLuint*)&meshDeformable->idVertices);
	glBindBuffer(GL_ARRAY_BUFFER, meshDeformable->idVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshDeformable->numVertices * 3, meshDeformable->vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, (GLuint*)&meshDeformable->idNormals);
	glBindBuffer(GL_ARRAY_BUFFER, meshDeformable->idNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshDeformable->numVertices * 3, meshDeformable->normals, GL_DYNAMIC_DRAW);
}

void ComponentMesh::UpdateDeformable()
{
	glBindBuffer(GL_ARRAY_BUFFER, meshDeformable->idVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshDeformable->numVertices * 3, meshDeformable->vertices, GL_DYNAMIC_DRAW);
}

void ComponentMesh::ResetDeformable()
{
	memcpy(meshDeformable->vertices, mesh->vertices, mesh->numVertices * 3 * sizeof(float));
	memcpy(meshDeformable->normals, mesh->normals, mesh->numVertices * 3 * sizeof(float));
}

Mesh* ComponentMesh::GetMeshDeformable() const
{
	return meshDeformable;
}

const float* ComponentMesh::GetVertices() const
{
	if (mesh != nullptr)
	{
		return mesh->vertices;
	}
	return 0;
}

const uint* ComponentMesh::GetIndices() const
{
	if (mesh != nullptr)
	{
		return mesh->indices;
	}
	return 0;
}

const float* ComponentMesh::GetNormals() const
{
	if (mesh != nullptr)
	{
		return mesh->normals;
	}
	return 0;
}

const float* ComponentMesh::GetColors() const
{
	if (mesh != nullptr)
	{
		return mesh->colors;
	}
	return 0;
}

const float* ComponentMesh::GetTexCoords() const
{
	if (mesh != nullptr)
	{
		return mesh->texCoords;
	}
	return 0;
}

