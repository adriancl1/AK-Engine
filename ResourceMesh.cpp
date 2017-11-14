#include "Application.h"
#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleTextures.h"

#include "imgui-1.51/imgui.h"
#include "Glew/include/glew.h"
#include "MathGeo/Geometry/Triangle.h"
#include "MathGeo/Math/float4x4.h"
#include "MathGeo/Geometry/LineSegment.h"

ResourceMesh::ResourceMesh(int UID) : Resource(UID, Resource_Mesh)
{
}

ResourceMesh::~ResourceMesh()
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

bool ResourceMesh::LoadInMemory()
{
	App->importer->LoadOwnFormat(std::to_string(UID).c_str(), this);
	return true;
}

bool ResourceMesh::UnloadFromMemory()
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
	if (idVertices > 0)
	{
		glDeleteBuffers(1, &idVertices);
		idVertices = 0;
	}
	if (idIndices > 0)
	{
		glDeleteBuffers(1, &idIndices);
		idIndices = 0;
	}
	if (idNormals > 0)
	{
		glDeleteBuffers(1, &idNormals);
		idNormals = 0;
	}
	if (idColors > 0)
	{
		glDeleteBuffers(1, &idColors);
		idColors = 0;
	}
	if (idTexCoords > 0)
	{
		glDeleteBuffers(1, &idTexCoords);
		idTexCoords = 0;
	}
	return true;
}
