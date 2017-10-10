#pragma once

#include "Component.h"
#include "MathGeo\Math\float3.h"
#include "MathGeo\Math\Quat.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(float3 pos, float3 scale, Quat rot, ComponentType type = Component_Transform) : Component(Component_Transform), position(pos), scale(scale), rotation(rot)
	{
		name = "Transform";
	}

	~ComponentTransform()
	{}

	float3 position;
	float3 scale;
	Quat rotation;

	void OnEditor()
	{
		if (ImGui::TreeNodeEx(name.c_str()))
		{
			ImGui::Text("Position: X=%.2f, Y=%.2f, Z=%.2f", position.x, position.y, position.z);
			ImGui::Text("Scale: X=%.2f, Y=%.2f, Z=%.2f", scale.x, scale.y, scale.z);
			ImGui::Text("Rotation: X=%.2f, Y=%.2f, Z=%.2f, W=%.2f", rotation.x, rotation.y, rotation.z, rotation.w);
			ImGui::TreePop();
		}
	}
};
