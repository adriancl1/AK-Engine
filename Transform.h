#pragma once
#include "Component.h"
#include "MathGeo\Math\float3.h"
#include "MathGeo\Math\Quat.h"

class ComponentTransform : public Component
{
	ComponentTransform(float3 pos, float3 scale, Quat rot, ComponentType type = Component_Transform) : Component(Component_Transform)
	{}

	float3 position;
	float3 scale;
	Quat rotation;
};
