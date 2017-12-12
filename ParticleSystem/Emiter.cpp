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
	//	DrawSphere();
		break;
	case E_BOX:
		break;
	case E_SEMISPHERE:
		break;
	case E_CONE:
		break;
	case E_SQUARE:
		break;
	case E_CIRCLE:
		break;
	default:
		break;
	}



}

void Emiter::DrawSphere(const Sphere & sphere)
{
	glColor3f(0.7f, 1.0f, 1.0f); // color blue azure

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
	glColor3f(0.7f, 1.0f, 1.0f);// color blue azure


}


EmiterData::EmiterData()
{
	transformation.Position = float3::zero;
	transformation.Rotation = Quat::identity;
	transformation.Scale = float3::zero;
}
