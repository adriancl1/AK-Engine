#include "ComponentCamera.h"

#include "Glew\include\glew.h"
#include "MathGeo\Math\MathAll.h"

ComponentCamera::ComponentCamera(float3 pos, float3 front, float3 up, float nearPlaneDistance, float farPlaneDistance, float verticalFov, float aspectRatio, FrustumType type) : Component(Component_Camera)
{
	name = "Camera";
	this->aspectRatio = aspectRatio;
	frustum.type = type;
	frustum.pos = pos;
	frustum.front = front;
	frustum.up = up;
	frustum.nearPlaneDistance = nearPlaneDistance;
	frustum.farPlaneDistance = farPlaneDistance;
	frustum.verticalFov = verticalFov;
	frustum.horizontalFov = 2 * Atan(Tan(verticalFov / 2) * (aspectRatio));
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

void ComponentCamera::SetVerticalFOV(float value)
{
	frustum.verticalFov = value;
	frustum.horizontalFov = 2 * Atan(Tan(value / 2) * (aspectRatio));
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
