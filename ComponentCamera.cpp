#include "Globals.h"
#include "ComponentCamera.h"

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
		ImGui::SliderFloat("X", &frustum.pos.x, -500, 500);
		ImGui::SliderFloat("Y", &frustum.pos.y, -500, 500);
		ImGui::SliderFloat("Z", &frustum.pos.y, -500, 500);
		ImGui::TreePop();

		ImGui::Text("FOV:");
		if(ImGui::SliderFloat("VFOV", &FOV, 1, 115))
		{
			SetFOV();
		}
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

void ComponentCamera::DrawDebug()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float3 vertices[8];
	frustum.GetCornerPoints(vertices);

	glColor3f(1.0f, 1.0f, 0.0f);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy, sz);
	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f( sx, -sy, sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f( sx,  sy, sz);
	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx,  sy, sz);

	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx,  sy, -sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f( sx,  sy, -sz);

	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f(sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f(sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f(sx,  sy, -sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f(sx,  sy,  sz);

	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx,  sy,  sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx,  sy, -sz);

	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx, sy,  sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f( sx, sy,  sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f( sx, sy, -sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx, sy, -sz);

	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f( sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy,  sz);

	glEnd();
}

bool ComponentCamera::Contains(const AABB & aabb) const
{
	return frustum.ContainsAaBox(aabb);
}
