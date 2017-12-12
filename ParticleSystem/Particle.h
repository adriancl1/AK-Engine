#ifndef _PS_PARTICLE_
#define _PS_PARTICLE_

#include "..\MathGeo\Math\float3.h"
#include "..\MathGeo\Math\float4.h"
#include "..\MathGeo\Math\Quat.h"

struct ParticleData
{
	float3 Position = float3::zero;					//Particle Actual Position
	Quat Rotation = Quat::identity;					//Particle Actual Rotation
	float3 Scale = float3::one;						//Particle Actual Scale
	float Size = 0.0f;								//Scale Multiplicator to modify
	float3 Speed = float3::zero;					//Particle Speed
	float3 Direction = float3::zero;		//Particle Emission Direction
	float3 force = float3::zero;					//Force that effects that particle
	float MaxLifeTime = 0;							//Max Particle Lifetime
	float LifeTime = 0;								//Actual Particle Lifetime
	uint TextureID = 0;								//Texture ID used by this particle
	float4 RGBA = float4::zero;
};

struct State 
{
	float3 force = float3::zero;					//Gravity that affects that particle
	float Size = 0.0f;
	float4 RGBA = float4::one;
};

class ParticleSystem;

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
	ParticleData Data;
	State Initial;
	State Final;

};





#endif // !_PS_PARTICLE_
