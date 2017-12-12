#ifndef _PS_EMITER_
#define _PS_EMITER_

#include "GlobalDefines.h"
#include "ParticleSystem.h"

#include "../Glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

//-------------------------------[ENUMS, SHAPES, UNIONS]

enum Etype //type of the emiter
{
	E_SPHERE,
	E_BOX,
	E_SEMISPHERE,
	E_CONE,
	E_SQUARE,
	E_CIRCLE
};

struct SCone
{
	Circle up;
	Circle down;
	float tall = 1.f;
};

union Shape
{
	Shape(){}

	AABB box;
	Sphere sphere;
	Sphere semiSphere;
	Circle circle;
	SCone cone;
	AABB quad;
}; 




//-------------------------------[EMITER]
class Emiter 	//shapes // sphere, cube, cone, semiphere
{
public:
	Emiter();
	~Emiter();
	
	void DrawEmiter();
	//draws

	//	E_SPHERE,E_BOX,	E_SEMISPHERE, E_CONE, E_SQUARE,	E_CIRCLE

	void DrawSphere(const Sphere&sphere);
	void DrawBox(const AABB& box);
	void DrawSemiSphere(const Sphere&sphere);
	void DrawCone(const SCone& cone);
	void DrawPoligon(const AABB& box);
	void DrawCircle(const Circle& circle);

private:
	bool active = true; //can be draw the shape or not
	Etype type;

	Shape shape; 
};

//-------------------------------[DATA]

struct EmiterData 
	//contains all data of the Emiter
{
	EmiterData(); //init transform
public:
	//emitor --------
	float timeToEmite = 0.f; // if the timeToEmite == 0.0f then never ends to emite == bool loop = true
	bool loop = true;

	int particleRate = 4; // quantity of particles droped in a sec
	
 	//Particles --------
	float timePLife = 2.f;
	float modTimePlife = 0.f; // intervale of time life of the particle

	float speed = 0.f; // speed to drop the particles
	float modSpeed = 0.f; // intervale of speed to drop particles
	
	PTransformation transformation;
};

#endif // !_PS_EMITER
