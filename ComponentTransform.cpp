#include "ComponentTransform.h"
#include "GameObject.h"

#include "imgui-1.51\imgui.h"

ComponentTransform::ComponentTransform(float3 pos, float3 scale, Quat rot, ComponentType type) : Component(Component_Transform), position(pos), newPosition(pos), scale(scale), rotation(rot)
{
	name = "Transform";
	needToUpdate = false;
	rotationEuler = rot.ToEulerXYZ();
	rotation = rot;
	globalTransformMatrix = float4x4::FromQuat(rot);
	globalTransformMatrix = float4x4::Scale(scale, float3(0, 0, 0)) * globalTransformMatrix;
	globalTransformMatrix.float4x4::SetTranslatePart(pos.x, pos.y, pos.z);

	localTransformMatrix = globalTransformMatrix;
}

ComponentTransform::~ComponentTransform()
{}

void ComponentTransform::Update()
{
	if (needToUpdate && myGO->isStatic == false)
	{
		position = newPosition;
		UpdateTrans();
		needToUpdate = false;		
	}
}

void ComponentTransform::UpdateTrans()
{
	rotation = Quat::FromEulerXYZ(rotationEuler.x, rotationEuler.y, rotationEuler.z);
	globalTransformMatrix = float4x4::FromQuat(rotation);
	globalTransformMatrix = float4x4::Scale(scale, float3(0,0,0)) * globalTransformMatrix;
	globalTransformMatrix.float4x4::SetTranslatePart(position.x, position.y, position.z);

	SetLocalTrans(myGO->GetParent());
	myGO->UpdateChildsTransform();

	needToUpdate = false;
}

void ComponentTransform::UpdateTransFromParent(GameObject * parent)
{
	ComponentTransform* parentTrans = (ComponentTransform*)parent->FindComponent(Component_Transform);
	if (parentTrans != nullptr)
	{
		globalTransformMatrix = parentTrans->localTransformMatrix * globalTransformMatrix;
		float4x4 temp;
		globalTransformMatrix.Decompose(newPosition, temp, scale);
		rotationEuler = temp.ToEulerXYZ();
		UpdateTrans();
	}
}

void ComponentTransform::SetLocalTrans(GameObject* parent)
{
	if (parent != nullptr)
	{
		ComponentTransform* parentTrans = (ComponentTransform*)parent->FindComponent(Component_Transform);
		if (parentTrans != nullptr)
		{
			float3 localPos = position - parentTrans->position;
			Quat localRot = rotation * parentTrans->rotation.Conjugated();
			float3 localScale = scale.Mul(parentTrans->scale.Recip());

			localTransformMatrix = float4x4::FromQuat(localRot);
			globalTransformMatrix = float4x4::Scale(localScale, float3(0, 0, 0)) * globalTransformMatrix;
			globalTransformMatrix.float4x4::SetTranslatePart(localPos.x, localPos.y, localPos.z);
		}
		else
		{
			localTransformMatrix = globalTransformMatrix;
		}
	}
	else
	{
		localTransformMatrix = globalTransformMatrix;
	}
}

void ComponentTransform::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
			if (myGO->isStatic)
			{
				ImGui::Text("Position:");
				ImGui::Text("X: %.2f",position.x);
				ImGui::SameLine;
				ImGui::Text("Y: %.2f", position.y);
				ImGui::SameLine;
				ImGui::Text("Z: %.2f", position.z);
				ImGui::Text("Scale", position.x);
				ImGui::Text("X: %.2f", scale.x);
				ImGui::SameLine;
				ImGui::Text("Y: %.2f", scale.y);
				ImGui::SameLine;
				ImGui::Text("Z: %.2f", scale.z);
				ImGui::Text("Rotation", position.x);
				ImGui::Text("X: %.2f", rotationEuler.x);
				ImGui::SameLine;
				ImGui::Text("Y: %.2f", rotationEuler.y);
				ImGui::SameLine;
				ImGui::Text("Z: %.2f", rotationEuler.z);
			}
			else
			{
				ImGui::Text("Position:");
				if (ImGui::SliderFloat("X", &newPosition.x, -500, 500))
				{
					needToUpdate = true;
				}
				if (ImGui::SliderFloat("Y", &newPosition.y, -10, 10))
				{
					needToUpdate = true;
				}
				if (ImGui::SliderFloat("Z", &newPosition.z, -10, 10))
				{
					needToUpdate = true;
				}

				ImGui::Text("Scale:");
				if (ImGui::SliderFloat("X##1", &scale.x, -10, 10))
				{
					needToUpdate = true;
				}
				if (ImGui::SliderFloat("Y##1", &scale.y, -10, 10))
				{
					needToUpdate = true;
				}
				if (ImGui::SliderFloat("Z##1", &scale.z, -10, 10))
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

				if (ImGui::Button("Reset"))
				{
					newPosition = float3::zero;
					scale = float3::one;
					rotationEuler = float3::zero;
					needToUpdate = true;
				}
			}

		ImGui::Checkbox("Static:", &myGO->isStatic);

		ImGui::TreePop();
	}
}

float4x4 ComponentTransform::GetTransMatrix() const
{
	return globalTransformMatrix;
}
