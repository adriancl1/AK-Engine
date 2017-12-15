#include "ComponentTransform.h"
#include "Application.h"
#include "GameObject.h"
#include "Configuration.h"
#include "Globals.h"
#include "ComponentCamera.h"

#include "imgui-1.51/imgui.h"
#include "ImGuizmo/ImGuizmo.h"

ComponentTransform::ComponentTransform(float3 pos, float3 scale, Quat rot, ComponentType type) : Component(Component_Transform), position(pos), newPosition(pos), scale(scale), rotation(rot)
{
	name = "Transform";
	needToUpdate = false;
	rotationEuler = rot.ToEulerXYZ();
	rotationEuler *= RADTODEG;
	rotation = rot;
	globalTransformMatrix = float4x4::FromTRS(position, rot, scale);
}

ComponentTransform::~ComponentTransform()
{}

void ComponentTransform::Update()
{
	if (needToUpdate)
	{
		UpdateTrans();
		needToUpdate = false;		
	}
	if (myGO->selected == true)
	{
		ShowGizmo(*App->camera->GetEditorCamera());
		//myGO->UpdateChildsTransform();
	}
}

void ComponentTransform::UpdateTrans()
{
	rotationEuler.x *= DEGTORAD;
	rotationEuler.y *= DEGTORAD;
	rotationEuler.z *= DEGTORAD;
	rotation = Quat::FromEulerXYZ(rotationEuler.x, rotationEuler.y, rotationEuler.z);
	globalTransformMatrix = float4x4::FromTRS(position, rotation, scale);

	rotationEuler.x *= RADTODEG;
	rotationEuler.y *= RADTODEG;
	rotationEuler.z *= RADTODEG;
	localTransformMatrix = globalTransformMatrix;
	if (myGO != nullptr)
	{
		ComponentTransform* parentTrans = (ComponentTransform*)myGO->GetParent()->FindComponent(Component_Transform);
		if (parentTrans != nullptr)
		{
			globalTransformMatrix = parentTrans->GetGlobalTransform() * globalTransformMatrix;
		}
	}

	needToUpdate = false;
	myGO->UpdateChildsTransform();
}

float4x4 ComponentTransform::GetGlobalTransform() const
{
	return globalTransformMatrix;
}

float4x4 ComponentTransform::GetLocalTransform() const
{
	return localTransformMatrix;
}

float3 ComponentTransform::GetPosition() const
{
	return position;
}

float3 ComponentTransform::GetGlobalPosition() const
{
	float3 pos, sca;
	Quat rot;
	globalTransformMatrix.Decompose(pos, rot, sca);
	return pos;
}

void ComponentTransform::SetPosition(float3 pos)
{
	position = pos;
	needToUpdate = true;
}

void ComponentTransform::SetRotation(Quat rot)
{
	rotation = rot;
	rotationEuler = rot.ToEulerXYZ();
	rotationEuler *= RADTODEG;
	needToUpdate = true;
}

void ComponentTransform::SetScale(float3 scale)
{
	this->scale = scale;
	needToUpdate = true;
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
				if (ImGui::DragFloat3("X", &position.x, 0.05f, NULL, NULL, "%.2f"))
				{
					needToUpdate = true;
				}
				ImGui::Text("Scale:");
				if (ImGui::DragFloat3("X##1", &scale.x, 0.05f, 1.0f, NULL, "%.2f"))
				{
					needToUpdate = true;
				}
				ImGui::Text("Rotation:");
				if (ImGui::DragFloat3("X##2", &rotationEuler.x, 0.05f, -180, 180, "%.2f"))
				{
					needToUpdate = true;
				}

				if (ImGui::Button("Reset"))
				{
					position = float3::zero;
					scale = float3::one;
					rotationEuler = float3::zero;
					needToUpdate = true;
				}
			}
			if (ImGui::Checkbox("Static:", &myGO->isStatic))
			{
				myGO->OnStaticChange();
			}
		ImGui::TreePop();
	}
}

void ComponentTransform::OnSave(Configuration& data) const
{
	data.SetInt("Type", type);
	data.AddArrayFloat("Position", &position.x, 3);
	data.AddArrayFloat("Rotation", &rotation.x, 4);
	data.AddArrayFloat("Scale", &scale.x, 3);
}

void ComponentTransform::OnLoad(Configuration & data)
{
	needToUpdate = false;
	position.x = data.GetFloat("Position", 0);
	position.y = data.GetFloat("Position", 1);
	position.z = data.GetFloat("Position", 2);
	rotation.x = data.GetFloat("Rotation", 0);
	rotation.y = data.GetFloat("Rotation", 1);
	rotation.z = data.GetFloat("Rotation", 2);
	rotation.w = data.GetFloat("Rotation", 3);
	rotationEuler = rotation.ToEulerXYZ();
	rotationEuler *= RADTODEG;
	scale.x = data.GetFloat("Scale", 0);
	scale.y = data.GetFloat("Scale", 1);
	scale.z = data.GetFloat("Scale", 2);
	
	globalTransformMatrix = float4x4::FromQuat(rotation);
	globalTransformMatrix = float4x4::Scale(scale, float3(0, 0, 0)) * globalTransformMatrix;
	globalTransformMatrix.float4x4::SetTranslatePart(position.x, position.y, position.z);

	localTransformMatrix = globalTransformMatrix;
}

void ComponentTransform::ShowGizmo(ComponentCamera & camera)
{
	ImGuizmo::Enable(true);
	if (myGO->isStatic == true)
	{
		ImGuizmo::Enable(false);
	}

	static ImGuizmo::OPERATION currentOperation(ImGuizmo::TRANSLATE);
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		currentOperation = ImGuizmo::TRANSLATE;
	}
	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		currentOperation = ImGuizmo::ROTATE;

	}
	else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		currentOperation = ImGuizmo::SCALE;
	}

	float* projMatrixg = camera.GetFrustum().ViewProjMatrix().Transposed().ptr();
	float* transMatrix = globalTransformMatrix.Transposed().ptr();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	ImGuizmo::Manipulate(float4x4::identity.ptr(), projMatrixg, currentOperation, ImGuizmo::WORLD, transMatrix);
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RALT) == KEY_IDLE && !ImGui::GetIO().WantCaptureKeyboard)
	{
		if (ImGuizmo::IsUsing())
		{
			ImGuizmo::DecomposeMatrixToComponents(transMatrix, (float*)position.ptr(), (float*)rotationEuler.ptr(), (float*)scale.ptr());
			globalTransformMatrix.Transpose();
			ImGuizmo::RecomposeMatrixFromComponents((float*)position.ptr(), (float*)rotationEuler.ptr(), (float*)scale.ptr(), globalTransformMatrix.ptr());
			globalTransformMatrix.Transpose();
		}
	}
}

float4x4 ComponentTransform::GetTransMatrix() const
{
	return globalTransformMatrix;
}
