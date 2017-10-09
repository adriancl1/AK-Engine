#pragma once
#include "Component.h"
#include "Globals.h"
#include "MathGeo\Geometry\AABB.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh() : Component(Component_Mesh)
	{}

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

	uint idTexture = 0; //id in VRAM

	AABB enclosingBox;

	void DrawDebug();

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
};