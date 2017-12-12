#include "Particle.h"
#include "ParticleSystem.h"



Particle::Particle(ParticleSystem * parent, const SystemState & initial, const SystemState & final, float3 Speed, float MaxLifeTime)
{

	Data.MaxLifeTime = MaxLifeTime;
	Data.Speed = Speed;
}


Particle::~Particle()
{
}

bool Particle::PreUpdate(float dt)
{
	//billboard

	float3 Orientation = pSystem->cameraPos - Data.Position;
	Orientation.y = Data.Position.y;
	Data.Rotation= Quat::LookAt(float3(0.0f, 0.0f, 1.0f), Orientation, float3(0.0f, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));
	return true;
}

bool Particle::Update(float dt) 
{
	Data.LifeTime += dt;
	CalcInterpolation();
	return true;
}

bool Particle::PostUpdate(float dt) 
{
	if (Data.LifeTime >= Data.MaxLifeTime) 
	{

	}
}

void Particle::CalcInterpolation() 
{
	//Position
	Data.Speed += Data.force*Data.LifeTime;
	Data.Position += Data.Speed*Data.LifeTime;

	//Size;
	Data.Size = Initial.Size + Data.LifeTime*(Final.Size - Initial.Size);

	//Color
	Data.RGBA.x = Initial.RGBA.x + Data.LifeTime*(Final.RGBA.x - Initial.RGBA.x);
	Data.RGBA.y = Initial.RGBA.y + Data.LifeTime*(Final.RGBA.y - Initial.RGBA.y);
	Data.RGBA.z = Initial.RGBA.z + Data.LifeTime*(Final.RGBA.z - Initial.RGBA.z);
	Data.RGBA.w = Initial.RGBA.w + Data.LifeTime*(Final.RGBA.w - Initial.RGBA.w);

	//Gravity

	Data.force.x = Initial.force.x + Data.LifeTime*(Final.force.x - Initial.force.x);
	Data.force.y = Initial.force.y + Data.LifeTime*(Final.force.y - Initial.force.y);
	Data.force.z = Initial.force.z + Data.LifeTime*(Final.force.z - Initial.force.z);
}