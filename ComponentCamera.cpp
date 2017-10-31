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
		if(ImGui::DragFloat3("VFOV", &FOV, 1, 115))
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

float* ComponentCamera::GetViewMatrix()
{
	return frustum.ViewProjMatrix().ptr();
}

void ComponentCamera::OnSave(Configuration& data) const
{
	data.SetInt("Type", type);
	data.SetFloat("Frustum Far", frustum.farPlaneDistance);
	data.SetFloat("Frustum Near", frustum.nearPlaneDistance);
	data.SetFloat("Frustum HFOV", frustum.horizontalFov);
	data.SetFloat("Frustum VFOV", frustum.verticalFov);
}
