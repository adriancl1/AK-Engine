#pragma once
#include "Resource.h"
#include "AnimationImporter.h"
#include "MathGeo/Math/float4x4.h"
#include "MathGeo/Math/Quat.h"

#include <string>
#include <vector>

class GameObject;
class ComponentAnimation;

struct PositionKey
{
	float3 value;
	float time;
};

struct RotationKey
{
	Quat value;
	float time;
};

struct AnimBone
{
	std::string name;
	std::vector<PositionKey> posKeys;
	std::vector<RotationKey> rotKeys;
};

class ResourceAnimation : public Resource
{
	friend AnimationImporter;
	friend ComponentAnimation;
public:
	ResourceAnimation(int UID);
	~ResourceAnimation();

	bool LoadInMemory()override;
	bool UnloadFromMemory()override;

private:
	std::vector<AnimBone> bones;
	float duration;
	int ticksPerSecond;
};
