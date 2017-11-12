#pragma once

#include "Resource.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh(int UID);
	~ResourceMesh();

	bool LoadInMemory()override;
	bool UnloadFromMemory()override;

	void Save(Configuration& config)const override;
	void Load(Configuration& config)override;

public:
	uint meshID = 0;

public:
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
};