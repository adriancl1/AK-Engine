#include "ComponentMesh.h"
#include "Primitive.h"

#include "imgui-1.51\imgui.h"
#include "Glew\include\glew.h"
#include "MathGeo\Geometry\Triangle.h"
#include "MathGeo\Math\float4x4.h"
ComponentMesh::ComponentMesh() : Component(Component_Mesh)
{
	name = "Mesh";
}

ComponentMesh::~ComponentMesh()
{
	if (vertices != nullptr)
	{
		delete[] vertices;
		vertices = nullptr;
	}
	if (indices != nullptr)
	{
		delete[] indices;
		indices = nullptr;
	}
	if (normals != nullptr)
	{
		delete[] normals;
		normals = nullptr;
	}
	if (colors != nullptr)
	{
		delete[] colors;
		colors = nullptr;
	}
	if (texCoords != nullptr)
	{
		delete[] texCoords;
		texCoords = nullptr;
	}
}

void ComponentMesh::Update()
{
}

float3 ComponentMesh::GetCenter() const
{
	return enclosingBox.Centroid();
}

void ComponentMesh::DrawDebug() const
{
	if (idNormals > 0)
	{
		for (int i = 0; i < numVertices * 3; i += 3)
		{
			pLine vNormal(vertices[i], vertices[i + 1], vertices[i + 2], vertices[i] + normals[i], vertices[i + 1] + normals[i + 1], vertices[i + 2] + normals[i + 2]);
			vNormal.color = Green;
			vNormal.Render();
		}
		for (int i = 0; i < numIndices; i += 3)
		{
			Triangle face(float3(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]), float3(vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]), float3(vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]));
			float3 faceCenter = face.Centroid();
			float3 faceNormal = face.NormalCCW();
			pLine normal(faceCenter.x, faceCenter.y, faceCenter.z, faceCenter.x + faceNormal.x, faceCenter.y + faceNormal.y, faceCenter.z + faceNormal.z);
			normal.color = Orange;
			normal.Render();
		}
	}

	//Draw enclosing box ----
	float3 corners[8];
	enclosingBox.GetCornerPoints(corners);

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
		ImGui::Text("Vertices ID: %i", idVertices);
		ImGui::Text("Num Vertices: %i", numVertices);
		ImGui::Text("Indexes ID: %i", idIndices);
		ImGui::Text("Num Indexes: %i", numIndices);
		ImGui::Text("Normals ID: %i", idNormals);
		ImGui::Text("Colors ID: %i", idColors);
		ImGui::Text("Texture Coords: %i", idTexCoords);
		ImGui::TreePop();
	}
}
