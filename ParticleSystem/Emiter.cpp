#include "Emiter.h"
#include "../MathGeo/MathGeoLib.h"
#include "../imgui-1.51/imgui.h"
#include "GlobalDefines.h"

Emiter::Emiter()
{
	type = E_SPHERE;
	DefaultEmiterData();
}

Emiter::~Emiter()
{

}

void Emiter::DefaultEmiterData()
/* -DefaultEmiterData- Used to put default sizes of the shapes ( you can change it from GlobalDefines) */
{
	switch (type)
	{
	case E_SPHERE:

		shape.sphere.r = PS_DEFAULT_SIZE_SPHERE*0.5f;
		break;

	case E_BOX:
		shape.box.maxPoint = float3(-PS_DEFAULT_SIZE_CUBE*0.5f, -PS_DEFAULT_SIZE_CUBE*0.5f, -PS_DEFAULT_SIZE_CUBE*0.5f);
		shape.box.minPoint = float3(PS_DEFAULT_SIZE_CUBE*0.5f, PS_DEFAULT_SIZE_CUBE*0.5f, PS_DEFAULT_SIZE_CUBE*0.5f);
		break;

	case E_SEMISPHERE:
		shape.semiSphere.r = PS_DEFAULT_SIZE_SEMISPHERE*0.5f;
		break;

	case E_CONE:
		shape.cone.up.r = PS_DEFAULT_SIZE_CONE_UP*0.5f;
		shape.cone.down.r = PS_DEFAULT_SIZE_CONE_DOWN*0.5f;
		shape.cone.tall = PS_DEFAULT_SIZE_CONE_TALL;
		break;

	case E_SQUARE:
		shape.quad.maxPoint = float3(PS_DEFAULT_SIZE_SQUARE *-0.5f, 0, -PS_DEFAULT_SIZE_SQUARE*0.5f);
		shape.quad.minPoint = float3(PS_DEFAULT_SIZE_SQUARE*0.5f, 0, PS_DEFAULT_SIZE_SQUARE* 0.5f);
		break;

	case E_CIRCLE:
		shape.circle.r = PS_DEFAULT_SIZE_CIRCLE*0.5f;
		break;
	}

}

void Emiter::DrawEmiter()
{
	if (!active)
		return;

	switch (type)
	{
	case E_SPHERE:	
		shape.sphere.Transform(pSystem->transformation->rotation);
		shape.sphere.pos = pSystem->transformation->position;

		DrawSphere(shape.sphere);
		break;

	case E_BOX:

		DrawBox(shape.box);

		break;

	case E_SEMISPHERE:

		shape.semiSphere.Transform(pSystem->transformation->rotation);
		shape.semiSphere.pos = pSystem->transformation->position;

		DrawSemiSphere(shape.semiSphere);
		break;

	case E_CONE:

		shape.cone.down.pos = pSystem->transformation->position;
		shape.cone.up.pos = pSystem->transformation->position;

		DrawCone(shape.cone);
		break;

	case E_SQUARE:
		
		DrawBox(shape.quad);
		break;

	case E_CIRCLE:

		shape.circle.pos = pSystem->transformation->position;

		DrawCircle(shape.circle);
		break;

	default:
		DrawBox(shape.box);
		break;
	}
}

void Emiter::DrawEmiterEditor()
{
	/* Enable Disable Draw*/
	ImGui::Checkbox("Draw Shape", &active);

	ImGui::SliderFloat("Emiting time", (float*)&data.timeToEmite, -1, 20);


	if (data.timeToEmite < 0)// in the case that the Emiting time is null the emiter will emite all time
		data.loop = true;
	
	ImGui::Checkbox("Loop", &data.loop);

	ImGui::SliderFloat("Particle Emittion rate", &data.particleRate, 0.02, 4);

	if (data.particleRate < 0)
		data.particleRate = 0;

	ImGui::DragFloat("Particle time life", &data.timePLife, 2.2f);
	ImGui::DragFloat("variance of the Particle time life", &data.modTimePlife, 0.1f);

	ImGui::DragFloat("Start Speed", &data.speed, 2.f);
	ImGui::DragFloat("variance of the Start speed", &data.modSpeed, 0.1f);

	/* Select the shape*/
	if (ImGui::Combo("Shape", (int*)&type, "Sphere\0Box\0SemiSphere\0Cone\0Square\0Circle")) DefaultEmiterData();

	switch (type)
	{
	case E_SPHERE:
		ImGui::SliderFloat("Radio", (float*)&shape.sphere.r, 0.25, 20);
		break;

	case E_BOX:
		ImGui::DragFloat3("Max Point", (float*)&shape.box.maxPoint, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat3("Min Point", (float*)&shape.box.minPoint, 0.1f, -100.0f, 100.0f);
		break;

	case E_SEMISPHERE:
		ImGui::SliderFloat("Radio", (float*)&shape.semiSphere.r, 0.25, 20);
		break;

	case E_CONE:
		ImGui::SliderFloat("Superior Circle Radio", (float*)&shape.cone.up.r, 0.25, 20);
		ImGui::SliderFloat("Inferior Circle Radio", (float*)&shape.cone.down.r, 0.25, 20);
		ImGui::SliderFloat("Height", (float*)&shape.cone.tall, 0.25, 20);
		break;

	case E_SQUARE:
		ImGui::DragFloat3("Max Point", (float*)&shape.box.maxPoint, 0.1f, 0, 100.0f);
		shape.box.maxPoint.y = 0.22f;
		ImGui::DragFloat3("Min Point", (float*)&shape.box.minPoint, 0.1f, 0, 100.0f);
		shape.box.minPoint.y = 0.2f;
		break;

	case E_CIRCLE:
		ImGui::SliderFloat("Radio", (float*)&shape.circle.r, 0.25, 20);
		break;
	}
}

void Emiter::DrawSphere(const Sphere & sphere)
{

	glLineWidth(2.0f);
	glColor3f(PS_DEBUG_COLOR_LINES);

	float totalRad = CIRCLEDEGREES / CIRCLEPERFECTION;
	totalRad *= DEGTORAD;

	//X
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLEPERFECTION; i++)
	{
		glVertex3f(cos(totalRad * i) *  sphere.r + sphere.pos.x, 0.0f + sphere.pos.y, sin(totalRad * i) *  sphere.r + sphere.pos.z);
	}		
	glEnd();

	//Y
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLEPERFECTION; i++)
	{
		glVertex3f(cos(totalRad * i) *  sphere.r + sphere.pos.x, sin(totalRad * i) *  sphere.r + sphere.pos.y, 0.0f + sphere.pos.z);
	}	
	glEnd();

	//Z
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLEPERFECTION; i++)
	{
		glVertex3f(0.0f + sphere.pos.x, sin(totalRad * i) *  sphere.r + sphere.pos.y, cos(totalRad * i) *  sphere.r + sphere.pos.z);
	}	
	glEnd();


	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

}

void Emiter::DrawBox(const AABB & box)
{

	float3 pos = pSystem->transformation->position;

	glLineWidth(2.0f);
	//box vertices
	float3 vertex0 = box.CornerPoint(0);
	float3 vertex1 = box.CornerPoint(1);
	float3 vertex2 = box.CornerPoint(2);
	float3 vertex3 = box.CornerPoint(3);
	float3 vertex4 = box.CornerPoint(4);
	float3 vertex5 = box.CornerPoint(5);
	float3 vertex6 = box.CornerPoint(6);
	float3 vertex7 = box.CornerPoint(7);

	glColor3f(PS_DEBUG_COLOR_LINES);

	glBegin(GL_LINES);

	//A
	glVertex3f(vertex0.x + pos.x, vertex0.y + pos.y, vertex0.z + pos.z);
	glVertex3f(vertex1.x + pos.x, vertex1.y + pos.y, vertex1.z + pos.z);

	//B
	glVertex3f(vertex0.x + pos.x, vertex0.y + pos.y, vertex0.z + pos.z);
	glVertex3f(vertex2.x + pos.x, vertex2.y + pos.y, vertex2.z + pos.z);

	//C
	glVertex3f(vertex0.x + pos.x, vertex0.y + pos.y, vertex0.z + pos.z);
	glVertex3f(vertex4.x + pos.x, vertex4.y + pos.y, vertex4.z + pos.z);

	//D
	glVertex3f(vertex7.x + pos.x, vertex7.y + pos.y, vertex7.z + pos.z);
	glVertex3f(vertex6.x + pos.x, vertex6.y + pos.y, vertex6.z + pos.z);

	//E
	glVertex3f(vertex7.x + pos.x, vertex7.y + pos.y, vertex7.z + pos.z);
	glVertex3f(vertex3.x + pos.x, vertex3.y + pos.y, vertex3.z + pos.z);

	//F
	glVertex3f(vertex7.x + pos.x, vertex7.y + pos.y, vertex7.z + pos.z);
	glVertex3f(vertex5.x + pos.x, vertex5.y + pos.y, vertex5.z + pos.z);

	//G
	glVertex3f(vertex5.x + pos.x, vertex5.y + pos.y, vertex5.z + pos.z);
	glVertex3f(vertex1.x + pos.x, vertex1.y + pos.y, vertex1.z + pos.z);

	//H
	glVertex3f(vertex5.x + pos.x, vertex5.y + pos.y, vertex5.z + pos.z);
	glVertex3f(vertex4.x + pos.x, vertex4.y + pos.y, vertex4.z + pos.z);

	//Y
	glVertex3f(vertex2.x + pos.x, vertex2.y + pos.y, vertex2.z + pos.z);
	glVertex3f(vertex3.x + pos.x, vertex3.y + pos.y, vertex3.z + pos.z);

	//J
	glVertex3f(vertex2.x + pos.x, vertex2.y + pos.y, vertex2.z + pos.z);
	glVertex3f(vertex6.x + pos.x, vertex6.y + pos.y, vertex6.z + pos.z);

	//K
	glVertex3f(vertex6.x + pos.x, vertex6.y + pos.y, vertex6.z + pos.z);
	glVertex3f(vertex4.x + pos.x, vertex4.y + pos.y, vertex4.z + pos.z);

	//L
	glVertex3f(vertex3.x + pos.x, vertex3.y + pos.y, vertex3.z + pos.z);
	glVertex3f(vertex1.x + pos.x, vertex1.y + pos.y, vertex1.z + pos.z);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);

}

void Emiter::DrawSemiSphere(const Sphere & sphere)
{

	glLineWidth(2.0f);
	glColor3f(PS_DEBUG_COLOR_LINES);

	
	float totalRad = CIRCLEDEGREES / CIRCLEPERFECTION;
	totalRad *= DEGTORAD;

	//X
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLEPERFECTION; i++)
	{
		glVertex3f(cos(totalRad * i) *  sphere.r + sphere.pos.x, 0.0f + sphere.pos.y, sin(totalRad * i) *  sphere.r + sphere.pos.z);
	}	
	glEnd();

	//Y
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i <= CIRCLEPERFECTION * 0.5f; i++)
	{
		glVertex3f(cos(totalRad * i) *  sphere.r + sphere.pos.x, sin(totalRad * i) *  sphere.r + sphere.pos.y, 0.0f + sphere.pos.z);
	}	
	glEnd();

	//Z
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i <= CIRCLEPERFECTION * 0.5f; i++)
	{
		glVertex3f(0.0f + sphere.pos.x, sin(totalRad * i) *  sphere.r + sphere.pos.y, cos(totalRad * i) *  sphere.r + sphere.pos.z);
	}
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);

}

void Emiter::DrawCone(const SCone & cone)
{

	float totalRad = CIRCLEDEGREES / CIRCLEPERFECTION;
	totalRad *= DEGTORAD;

	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLEPERFECTION; i++)
	{
		glVertex3f(cos(totalRad * i) * cone.up.r + cone.up.pos.x, cone.tall + cone.up.pos.y, sin(totalRad * i) * cone.up.r + cone.up.pos.z);
	}
	glEnd();

	DrawCircle(cone.down);

	glLineWidth(2.0f);
	glColor3f(PS_DEBUG_COLOR_LINES);

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f+ cone.down.pos.x, 0.0f + cone.down.pos.y, -cone.down.r + cone.down.pos.z);
	glVertex3f(0.0f + cone.down.pos.x, cone.tall + cone.down.pos.y, -cone.up.r + cone.down.pos.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f + cone.down.pos.x, cone.tall + cone.down.pos.y, cone.up.r + cone.down.pos.z);
	glVertex3f(0.0f + cone.down.pos.x, 0.0f + cone.down.pos.y, cone.down.r + cone.down.pos.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(cone.down.r + cone.down.pos.x, 0.0f + cone.down.pos.y, 0.0f + cone.down.pos.z);
	glVertex3f(cone.up.r + cone.down.pos.x, cone.tall + cone.down.pos.y, 0.0f + cone.down.pos.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(-cone.up.r + cone.down.pos.x, cone.tall + cone.down.pos.y, 0.0f + cone.down.pos.z);
	glVertex3f(-cone.down.r + cone.down.pos.x, 0.0f + cone.down.pos.y, 0.0f + cone.down.pos.z);
	glEnd();

	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

}

void Emiter::DrawPoligon(const AABB & box)
{
	glLineWidth(2.0f);
	//quad vertices
	float3 vertex0 = box.CornerPoint(0);
	float3 vertex1 = box.CornerPoint(1);
	float3 vertex2 = box.CornerPoint(2);
	float3 vertex3 = box.CornerPoint(3);

	//A
	glVertex3f(vertex0.x, vertex0.y, vertex0.z);
	glVertex3f(vertex1.x, vertex1.y, vertex1.z);
	//B
	glVertex3f(vertex0.x, vertex0.y, vertex0.z);
	glVertex3f(vertex3.x, vertex3.y, vertex3.z);
	//C
	glVertex3f(vertex1.x, vertex1.y, vertex1.z);
	glVertex3f(vertex2.x, vertex2.y, vertex2.z);
	//D
	glVertex3f(vertex2.x, vertex2.y, vertex2.z);
	glVertex3f(vertex3.x, vertex3.y, vertex3.z);
	
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);


}

void Emiter::DrawCircle(const Circle & circle)
{

	glLineWidth(2.0f);
	glColor3f(PS_DEBUG_COLOR_LINES);


	float totalRad = CIRCLEDEGREES / CIRCLEPERFECTION;
	totalRad *= DEGTORAD;

	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLEPERFECTION; i++)
	{
		glVertex3f(cos(totalRad * i) *  circle.r + circle.pos.x, 0.0f + circle.pos.y, sin(totalRad * i) *  circle.r+ circle.pos.z);
	}
	glEnd();

	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
}

EmiterData::EmiterData()
{
	
}
