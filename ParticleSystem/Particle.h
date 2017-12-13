#ifndef _PS_PARTICLE_
#define _PS_PARTICLE_

#include "..\MathGeo\Math\float3.h"
#include "..\MathGeo\Math\float4.h"
#include "..\MathGeo\Math\Quat.h"
#include "ParticleSystem.h"

struct ParticleData
{
	float3 Position = float3::zero;				
	Quat Rotation = Quat::identity;					
	float3 Scale = float3::one;					
	float Size = 0.0f;							
	float3 Speed = float3::zero;				
	float3 Direction = float3::zero;			
	float3 gravity = float3::zero;
	float MaxLifeTime = 0;							
	float LifeTime = 0;							
	uint TextureID = 0;							
	float4 RGBA = float4::zero;
};

struct State 
{
	float3 gravity = float3::zero;
	float Size = 0.0f;
	float4 RGBA = float4::one;
};

class Particle
{
public:

	Particle(ParticleSystem* parent, const SystemState& initial , const SystemState& final, float3 Speed, float MaxLifeTime);
	~Particle();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void CalcInterpolation();

private:
	ParticleSystem * pSystem;
	ParticleData data;
	State Initial;
	State Final;

};





#endif // !_PS_PARTICLE_
