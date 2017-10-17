#include "ComponentTransform.h"
#include "GameObject.h"

#include "imgui-1.51\imgui.h"

ComponentTransform::ComponentTransform(float3 pos, float3 scale, Quat rot, ComponentType type) : Component(Component_Transform), position(pos), scale(scale), rotation(rot)
{
	name = "Transform";
	needToMove = false;
}

ComponentTransform::~ComponentTransform()
{}

void ComponentTransform::Update()
{
	if (needToMove && myGO != nullptr)
	{
		myGO->Move(position);
		needToMove = false;
	}
}

void ComponentTransform::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Position:");
		if (ImGui::SliderFloat("X", &position.x, -500, 500))
		{
			needToMove = true;
		}
		if (ImGui::SliderFloat("Y", &position.y, -500, 500))
		{
			needToMove = true;
		}
		if (ImGui::SliderFloat("Z", &position.z, -500, 500))
		{
			needToMove = true;
		}

		ImGui::Text("Scale: X=%.2f, Y=%.2f, Z=%.2f", scale.x, scale.y, scale.z);
		ImGui::Text("Rotation: X=%.2f, Y=%.2f, Z=%.2f, W=%.2f", rotation.x, rotation.y, rotation.z, rotation.w);
		ImGui::TreePop();
	}
}