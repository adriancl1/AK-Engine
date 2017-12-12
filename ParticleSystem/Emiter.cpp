#include "Emiter.h"
#include "../MathGeo/MathGeoLib.h"


Emiter::Emiter()
{

}

Emiter::~Emiter()
{

}

void Emiter::DrawEmiter()
{
	switch (type)
	{
	case E_SPHERE:	
		DrawSphere(shape.sphere);
		break;

	case E_BOX:
		DrawBox(shape.box);
		break;

	case E_SEMISPHERE:
		DrawSemiSphere(shape.semiSphere);
		break;

	case E_CONE:
		DrawCone(shape.cone);
		break;

	case E_SQUARE:
		DrawPoligon(shape.quad);
		break;

	case E_CIRCLE:
		DrawCircle(shape.circle);
		break;

	default:
		DrawBox(shape.box);
		break;
	}
}

void Emiter::DrawSphere(const Sphere & sphere)
{

	glLineWidth(2.0f);
	glColor3f(DEBUG_COLOR_LINES); 

	float totalRad = CIRCLEDEGREES / CIRCLEPERFECTION;
	totalRad *= DEGTORAD;

	//X
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLEPERFECTION; i++)
	{
		glVertex3f(cos(totalRad * i) *  sphere.r, 0.0f, sin(totalRad * i) *  sphere.r);
	}		
	glEnd();

	//Y
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLEPERFECTION; i++)
	{
		glVertex3f(cos(totalRad * i) *  sphere.r, sin(totalRad * i) *  sphere.r, 0.0f);
	}	
	glEnd();

	//Z
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLEPERFECTION; i++)
	{
		glVertex3f(0.0f, sin(totalRad * i) *  sphere.r, cos(totalRad * i) *  sphere.r);
	}	
	glEnd();


	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

}

void Emiter::DrawBox(const AABB & box)
{

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

	glColor3f(DEBUG_COLOR_LINES);

	glBegin(GL_LINES);

	//A
	glVertex3f(vertex0.x, vertex0.y, vertex0.z);
	glVertex3f(vertex1.x, vertex1.y, vertex1.z);

	//B
	glVertex3f(vertex0.x, vertex0.y, vertex0.z);
	glVertex3f(vertex2.x, vertex2.y, vertex2.z);

	//C
	glVertex3f(vertex0.x, vertex0.y, vertex0.z);
	glVertex3f(vertex4.x, vertex4.y, vertex4.z);

	//D
	glVertex3f(vertex7.x, vertex7.y, vertex7.z);
	glVertex3f(vertex6.x, vertex6.y, vertex6.z);

	//E
	glVertex3f(vertex7.x, vertex7.y, vertex7.z);
	glVertex3f(vertex3.x, vertex3.y, vertex3.z);

	//F
	glVertex3f(vertex7.x, vertex7.y, vertex7.z);
	glVertex3f(vertex5.x, vertex5.y, vertex5.z);

	//G
	glVertex3f(vertex5.x, vertex5.y, vertex5.z);
	glVertex3f(vertex1.x, vertex1.y, vertex1.z);

	//H
	glVertex3f(vertex5.x, vertex5.y, vertex5.z);
	glVertex3f(vertex4.x, vertex4.y, vertex4.z);

	//Y
	glVertex3f(vertex2.x, vertex2.y, vertex2.z);
	glVertex3f(vertex3.x, vertex3.y, vertex3.z);

	//J
	glVertex3f(vertex2.x, vertex2.y, vertex2.z);
	glVertex3f(vertex6.x, vertex6.y, vertex6.z);

	//K
	glVertex3f(vertex6.x, vertex6.y, vertex6.z);
	glVertex3f(vertex4.x, vertex4.y, vertex4.z);

	//L
	glVertex3f(vertex3.x, vertex3.y, vertex3.z);
	glVertex3f(vertex1.x, vertex1.y, vertex1.z);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);

}

void Emiter::DrawSemiSphere(const Sphere & sphere)
{

	glLineWidth(2.0f);
	glColor3f(DEBUG_COLOR_LINES);

	
	float totalRad = CIRCLEDEGREES / CIRCLEPERFECTION;
	totalRad *= DEGTORAD;

	//X
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < CIRCLEPERFECTION; i++)
	{
		glVertex3f(cos(totalRad * i) *  sphere.r, 0.0f, sin(totalRad * i) *  sphere.r);
	}	
	glEnd();

	//Y
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i <= CIRCLEPERFECTION * 0.5f; i++)
	{
		glVertex3f(cos(totalRad * i) *  sphere.r, sin(totalRad * i) *  sphere.r, 0.0f);
	}	
	glEnd();

	//Z
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i <= CIRCLEPERFECTION * 0.5f; i++)
	{
		glVertex3f(0.0f, sin(totalRad * i) *  sphere.r, cos(totalRad * i) *  sphere.r);
	}
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);

}

void Emiter::DrawCone(const SCone & cone)
{

	DrawCircle(cone.up);
	DrawCircle(cone.down);

	glLineWidth(2.0f);
	glColor3f(DEBUG_COLOR_LINES);

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f, 0.0f, -cone.down.r);
	glVertex3f(0.0f, cone.tall, -cone.up.r);
	glVertex3f(0.0f, cone.tall, cone.up.r);
	glVertex3f(0.0f, 0.0f, cone.down.r);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(cone.down.r, 0.0f, 0.0f);
	glVertex3f(cone.up.r, cone.tall, 0.0f);
	glVertex3f(-cone.up.r, cone.tall, 0.0f);
	glVertex3f(-cone.down.r, 0.0f, 0.0f);
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
	glColor3f(DEBUG_COLOR_LINES);


	float totalRad = CIRCLEDEGREES / CIRCLEPERFECTION;
	totalRad *= DEGTORAD;

	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i <= CIRCLEPERFECTION * 0.5f; i++)
	{
		glVertex3f(0.0f, sin(totalRad * i) *  circle.r, cos(totalRad * i) *  circle.r);
	}
	glEnd();

	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
}



EmiterData::EmiterData()
{
	transformation.Position = float3::zero;
	transformation.Rotation = Quat::identity;
	transformation.Scale = float3::zero;
}
