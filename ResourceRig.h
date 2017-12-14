#pragma once
#include "Resource.h"
#include "RigImporter.h"
#include "MathGeo/Math/float4x4.h"

#include <string>
#include <vector>

class GameObject;

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

class ResourceRig: public Resource
{
	friend RigImporter;
public:
	ResourceRig(int UID);
	~ResourceRig();

	bool LoadToGameObject(GameObject* GO, std::vector<GameObject*>* toFill);
	bool LoadInMemory()override;
	bool UnloadFromMemory()override;

	int GetBoneNum() const;

private:
	std::vector<Bone> bones;
};
