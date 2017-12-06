#pragma once
#include "Resource.h"
#include "RigImporter.h"
#include "MathGeo/Math/float4x4.h"

#include <string>
#include <vector>


struct VertexWeight
{
	int vertexID;
	float weight;
};

struct Bone
{
	std::string name;
	float4x4 offsetMatrix;
	std::vector<VertexWeight> weights;
};

class ResourceRig :public Resource
{
	friend RigImporter;
public:
	ResourceRig(int UID);
	~ResourceRig();

	bool LoadInMemory()override;
	bool UnloadFromMemory()override;

private:
	std::vector<Bone> bones;
};
