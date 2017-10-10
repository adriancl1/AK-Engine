#pragma once

#include "Component.h"
#include "Globals.h"
#include "Primitive.h"
#include "Glew\include\glew.h"
#include "MathGeo\Geometry\Triangle.h"
#include "MathGeo\Math\float4x4.h"
#include "MathGeo\Geometry\AABB.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh() : Component(Component_Mesh)
	{
		name = "Mesh";
	}

	~ComponentMesh()
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
	uint idVertices = 0; // id in VRAM 
	uint numVertices = 0;
	float* vertices = nullptr;

	uint idIndices = 0; // id in VRAM 
	uint numIndices = 0;
	uint* indices = nullptr;

	uint idNormals = 0; // id in VRAM
	float* normals = nullptr;

	uint idColors = 0; // id in VRAM
	float* colors = nullptr;

	uint idTexCoords = 0; // id in VRAM
	float* texCoords = nullptr;

	AABB enclosingBox;

	void Update()
	{

	}

	void DrawDebug()
	{
		if (idNormals>0)
		{
			for (int i = 0; i < numVertices; i += 3)
			{
				pLine n(vertices[i], vertices[i + 1], vertices[i + 2], normals[i] + vertices[i], normals[i + 1] + vertices[i + 1], normals[i + 2] + vertices[i + 2]);
				n.color = Green;
				n.Render();
			}
			for (int i = 0; i < numVertices; i += 9)
			{
				Triangle face(float3(vertices[i], vertices[i + 1], vertices[i + 2]), float3(vertices[i + 3], vertices[i + 4], vertices[i + 5]), float3(vertices[i + 6], vertices[i + 7], vertices[i + 8]));
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

		glPushMatrix();

		glMultMatrixf((GLfloat*)float4x4::identity.Transposed().ptr());
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
		glPopMatrix();

		glColor3f(0, 0, 0);

	}

	void OnEditor() override
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
};