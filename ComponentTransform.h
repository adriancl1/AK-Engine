#pragma once

#include "Component.h"
#include "MathGeo\Math\float3.h"
#include "MathGeo\Math\Quat.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(float3 pos, float3 scale, Quat rot, ComponentType type = Component_Transform) : Component(Component_Transform)
	{}

	~ComponentTransform()
	{}

	float3 position;
	float3 scale;
	Quat rotation;

	void OnEditor()
	{
		ImGui::Text("Position: X=%i, Y=%i, Z=%i", position.x, position.y, position.z);
		ImGui::Text("Scale: X=%i, Y=%i, Z=%i", scale.x, scale.y, scale.z);
		ImGui::Text("Rotation: X=%i, Y=%i, Z=%i, W=%i", rotation.x, rotation.y, rotation.z, rotation.w);
	}
};
