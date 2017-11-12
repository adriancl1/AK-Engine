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
}

float3 ComponentMesh::GetCenter() const
{
	return 	mesh->enclosingBox.Centroid();
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
			for (int i = 0; i < 	mesh->numIndices; i += 3)
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
		ImGui::Text("Vertices ID: %i", mesh->idVertices);
		ImGui::Text("Num Vertices: %i", mesh->numVertices);
		ImGui::Text("Indexes ID: %i", mesh->idIndices);
		ImGui::Text("Num Indexes: %i", mesh->numIndices);
		ImGui::Text("Normals ID: %i", mesh->idNormals);
		ImGui::Text("Colors ID: %i", mesh->idColors);
		ImGui::Text("Texture Coords: %i", mesh->idTexCoords);
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
	std::string myName = myGO->GetName();
	data.SetString("MeshFile", myName.c_str());
}

void ComponentMesh::OnLoad(Configuration & data)
{
	//App->importer->LoadOwnFormat(data.GetString("MeshFile"), this);
}

void ComponentMesh::AddResource(int UID)
{
	mesh = (ResourceMesh*)App->resources->Get(UID);
	mesh->LoadToComponent();
}

uint ComponentMesh::GetIDVertices() const
{
	return mesh->idVertices;
}

uint ComponentMesh::GetNumVertices() const
{
	return mesh->numVertices;
}

uint ComponentMesh::GetIDIndices() const
{
	return mesh->idIndices;
}

uint ComponentMesh::GetNumIndices() const
{
	return mesh->numIndices;
}

uint ComponentMesh::GetIDNormals() const
{
	return mesh->idNormals;
}

uint ComponentMesh::GetIDColors() const
{
	return mesh->idColors;
}

uint ComponentMesh::GetIDTextCoords() const
{
	return mesh->idTexCoords;
}

AABB ComponentMesh::GetEnclosingBox() const
{
	return mesh->enclosingBox;
}

const float* ComponentMesh::GetVertices() const
{
	return mesh->vertices;
}

const uint* ComponentMesh::GetIndices() const
{
	return mesh->indices;
}

const float* ComponentMesh::GetNormals() const
{
	return mesh->normals;
}

const float* ComponentMesh::GetColors() const
{
	return mesh->colors;
}

const float* ComponentMesh::GetTexCoords() const
{
	return mesh->texCoords;
}

