#include "ComponentTransform.h"
#include "GameObject.h"

#include "imgui-1.51\imgui.h"

ComponentTransform::ComponentTransform(float3 pos, float3 scale, Quat rot, ComponentType type) : Component(Component_Transform), position(pos), newPosition(pos), scale(scale), rotation(rot)
{
	name = "Transform";
	needToUpdate = false;
	rotationEuler = rot.ToEulerXYZ();
	transformMatrix = float4x4::FromQuat(rot);
	transformMatrix = float4x4::Scale(scale, float3(0, 0, 0)) * transformMatrix;
	transformMatrix.float4x4::SetTranslatePart(pos.x, pos.y, pos.z);
}

ComponentTransform::~ComponentTransform()
{}

void ComponentTransform::Update()
{
	if (needToUpdate)
	{
		UpdateTrans();
		needToUpdate = false;
		position = newPosition;
	}
}

void ComponentTransform::UpdateTrans()
{
	rotation = Quat::FromEulerXYZ(rotationEuler.x, rotationEuler.y, rotationEuler.z);
	transformMatrix = float4x4::FromQuat(rotation);
	transformMatrix = float4x4::Scale(scale, float3(0,0,0)) * transformMatrix;
	transformMatrix.float4x4::SetTranslatePart(position.x, position.y, position.z);
	needToUpdate = false;
}

void ComponentTransform::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Position:");
		if (ImGui::SliderFloat("X", &newPosition.x, -500, 500))
		{
			needToUpdate = true;
		}
		if (ImGui::SliderFloat("Y", &newPosition.y, -500, 500))
		{
			needToUpdate = true;
		}
		if (ImGui::SliderFloat("Z", &newPosition.z, -500, 500))
		{
			needToUpdate = true;
		}

		ImGui::Text("Scale:");
		if (ImGui::SliderFloat("X##1", &scale.x, -500, 500))
		{
			needToUpdate = true;
		}
		if (ImGui::SliderFloat("Y##1", &scale.y, -500, 500))
		{
			needToUpdate = true;
		}
		if (ImGui::SliderFloat("Z##1", &scale.z, -500, 500))
		{
			needToUpdate = true;
		}
		
		ImGui::Text("Rotation:");
		if (ImGui::SliderFloat("X##2", &rotationEuler.x, -360, 360))
		{
			needToUpdate = true;
		}
		if (ImGui::SliderFloat("Y##2", &rotationEuler.y, -360, 360))
		{
			needToUpdate = true;
		}
		if (ImGui::SliderFloat("Z##2", &rotationEuler.z, -360, 360))
		{
			needToUpdate = true;
		}

		ImGui::TreePop();
	}
}

float4x4 ComponentTransform::GetTransMatrix() const
{
	return transformMatrix;
}
