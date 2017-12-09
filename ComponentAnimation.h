#pragma once

#include "Globals.h"
#include "Component.h"

#include "MathGeo/Math/float3.h"

class ResourceAnimation;

class ComponentAnimation : public Component
{
public:
	ComponentAnimation();
	~ComponentAnimation();

	void Update()override;

	void OnEditor()override;

	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;
	void AddResource(int UID) override;

	void SetBonePos(float3 value)const;

private:
	ResourceAnimation* anim;
	float animTimer = 0;
};