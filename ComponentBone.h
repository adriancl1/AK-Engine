#pragma once

#include "Component.h"
#include "ResourceRig.h"
#include "Timer.h"

class ComponentMesh;

class ComponentBone : public Component
{
public:
	ComponentBone(Bone bone, ComponentMesh* mesh = nullptr);
	~ComponentBone();

	void Update();

	void OnEditor() override;

	void DrawDebug() const;

private:
	Bone bone;
	ComponentMesh* attachedMesh = nullptr;
	float4x4 originalTrans;
	float3 lastDiff = float3::zero;
	Timer toCheck;
	bool debug = false;
};