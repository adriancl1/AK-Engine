#include "Particle.h"
#include "ParticleSystem.h"



Particle::Particle(ParticleSystem * parent, const SystemState & initial, const SystemState & final, float3 Speed, float MaxLifeTime)
{

	data.MaxLifeTime = MaxLifeTime;
	data.Speed = Speed;
}


Particle::~Particle()
{
}

bool Particle::PreUpdate(float dt)
{
	//billboard

	float3 Orientation = pSystem->cameraPos - data.Position;
	Orientation.y = data.Position.y;
	data.Rotation= Quat::LookAt(float3(0.0f, 0.0f, 1.0f), Orientation, float3(0.0f, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));
	return true;
}

bool Particle::Update(float dt) 
{
	data.LifeTime += dt;
	CalcInterpolation();
	return true;
}

bool Particle::PostUpdate(float dt) 
{
	if (data.LifeTime >= data.MaxLifeTime)
	{

	}

	return true;
}

void Particle::CalcInterpolation() 
{
	//Position
	data.Speed += data.force*data.LifeTime;
	data.Position += data.Speed*data.LifeTime;

	//Size;
	data.Size = Initial.Size + data.LifeTime*(Final.Size - Initial.Size);

	//Color
	data.RGBA.x = Initial.RGBA.x + data.LifeTime*(Final.RGBA.x - Initial.RGBA.x);
	data.RGBA.y = Initial.RGBA.y + data.LifeTime*(Final.RGBA.y - Initial.RGBA.y);
	data.RGBA.z = Initial.RGBA.z + data.LifeTime*(Final.RGBA.z - Initial.RGBA.z);
	data.RGBA.w = Initial.RGBA.w + data.LifeTime*(Final.RGBA.w - Initial.RGBA.w);

	//Gravity

	data.force.x = Initial.force.x + data.LifeTime*(Final.force.x - Initial.force.x);
	data.force.y = Initial.force.y + data.LifeTime*(Final.force.y - Initial.force.y);
	data.force.z = Initial.force.z + data.LifeTime*(Final.force.z - Initial.force.z);
}