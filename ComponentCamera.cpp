#include "Globals.h"
#include "ComponentCamera.h"
#include "Configuration.h"

#include "imgui-1.51\imgui.h"
#include "Glew\include\glew.h"
#include "MathGeo\Math\MathAll.h"

ComponentCamera::ComponentCamera(float3 pos, float3 front, float3 up, float nearPlaneDistance, float farPlaneDistance, float verticalFov, float aspectRatio, FrustumType type) : Component(Component_Camera)
{
	name = "Camera";
	this->aspectRatio = aspectRatio;
	this->aspectRatio = (float)16 / 9;
	frustum.type = type;
	frustum.pos = pos;
	frustum.front = front;
	frustum.up = up;
	frustum.nearPlaneDistance = nearPlaneDistance;
	frustum.farPlaneDistance = farPlaneDistance;
	FOV = verticalFov;
	frustum.verticalFov = DEGTORAD * FOV;
	frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspectRatio));

	frustum.ProjectionMatrix();

	frustumCulling = true;

	active = false;
}

ComponentCamera::~ComponentCamera()
{}

void ComponentCamera::Update()
{
	DrawDebug();
}

void ComponentCamera::SetPos(float3 newPos)
{
	frustum.pos = newPos;
}

void ComponentCamera::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Position:");
		ImGui::DragFloat("X", &frustum.pos.x, -0.5f, 0.5f);
		ImGui::DragFloat("Y", &frustum.pos.y, -0.5f, 0.5f);
		ImGui::DragFloat("Z", &frustum.pos.z, -0.5f, 0.5f);

		ImGui::DragFloat3("Frustum up", frustum.up.ptr(), -0.5f, 0.5f);
		ImGui::DragFloat3("Frustum front", frustum.front.ptr(), -0.5f, 0.5f);

		ImGui::TreePop();

		ImGui::Text("FOV:");
		if(ImGui::DragFloat("VFOV", &FOV, 1, 115))
		{
			SetFOV();
		}
		ImGui::Checkbox("Active", &active);
		ImGui::Checkbox("Use Culling", &frustumCulling);
	}
}

//Sets the horizontal FOV according to current vertical
void ComponentCamera::SetFOV()
{
	frustum.verticalFov = DEGTORAD * FOV;
	frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspectRatio));
}

//Sets vertical FOV to value and horizontal according to aspect ratio
void ComponentCamera::SetVerticalFOV(float value)
{
	frustum.verticalFov = value;
	frustum.horizontalFov = 2 * Atan(Tan(value * 0.5f) * (aspectRatio));
}

void ComponentCamera::UpdateCamera(float3 pos, float3 front, float3 up)
{
	frustum.pos = pos;
	frustum.front = front;
	frustum.up = up;
}

void ComponentCamera::DrawDebug() const
{
	frustum.DrawDebug(Yellow);
}

bool ComponentCamera::Contains(const AABB & aabb) const
{
	return frustum.ContainsAaBox(aabb);
}

bool ComponentCamera::GetFrustumCulling() const
{
	return frustumCulling;
}

const float* ComponentCamera::GetViewMatrix() const
{
	return frustum.ViewProjMatrix().Transposed().ptr();
}

void ComponentCamera::OnSave(Configuration& data) const
{
	data.SetInt("Type", type);
	data.SetBool("Active", active);
	data.SetBool("Culling", frustumCulling);
	data.SetFloat("Aspect Ratio", aspectRatio);
	data.SetFloat("FOV", FOV);
	data.SetFloat("Frustum Far", frustum.farPlaneDistance);
	data.SetFloat("Frustum Near", frustum.nearPlaneDistance);
	data.SetFloat("Frustum HFOV", frustum.horizontalFov);
	data.SetFloat("Frustum VFOV", frustum.verticalFov);
	data.AddArrayFloat("Position", frustum.pos.ptr(), 3);
	data.AddArrayFloat("Frustum Up", frustum.up.ptr(), 3);
	data.AddArrayFloat("Frustum Front", frustum.front.ptr(), 3);
}

void ComponentCamera::OnLoad(Configuration & data)
{
	active = data.GetBool("Active");
	frustumCulling = data.GetBool("Culling");
	aspectRatio = data.GetFloat("Aspect Ratio");
	FOV = data.GetFloat("FOV");
	frustum.farPlaneDistance = data.GetFloat("Frustum Far");
	frustum.nearPlaneDistance = data.GetFloat("Frustum Near");
	frustum.horizontalFov = data.GetFloat("Frustum HFOV");
	frustum.verticalFov = data.GetFloat("Frustum VFOV");
	frustum.pos.x = data.GetFloat("Position", 0);
	frustum.pos.y = data.GetFloat("Position", 1);
	frustum.pos.z = data.GetFloat("Position", 2);
	frustum.up.x = data.GetFloat("Frustum Up", 0);
	frustum.up.y = data.GetFloat("Frustum Up", 1);
	frustum.up.z = data.GetFloat("Frustum Up", 2);
	frustum.front.x = data.GetFloat("Frustum Front", 0);
	frustum.front.x = data.GetFloat("Frustum Front", 0);
	frustum.front.x = data.GetFloat("Frustum Front", 0);

	frustum.ProjectionMatrix();
}
