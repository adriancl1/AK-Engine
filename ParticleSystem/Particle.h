#ifndef _PS_PARTICLE_
#define _PS_PARTICLE_

#include "..\MathGeo\Math\float3.h"
#include "..\MathGeo\Math\float4.h"
#include "..\MathGeo\Math\Quat.h"
#include "ParticleSystem.h"
#include "..\Glew\include\glew.h"


struct ParticleData
{
	float3 position = float3::zero;				
	Quat rotation = Quat::identity;					
	float3 scale = float3::one;					
	float size = 0.0f;							
	float speed = 0.0f;				
	float3 direction = float3::zero;			
	float gravity = -9.8f;
	float maxLifeTime = 0;							
	float lifeTime = 0;							
	uint textureID = 0;							
	float4 color = float4::zero;
};

struct ParticleMesh
{

	void SetMesh(ParticleMesh& newMesh);
	uint numFaces = 0;
	uint idVertices = 0; 
	uint numVertices = 0;
	float* vertices = nullptr;

	uint idIndices = 0; 
	uint numIndices = 0;
	uint* indices = nullptr;

	uint idNormals = 0; 
	float* normals = nullptr;

	uint idColors = 0; 
	float* colors = nullptr;

	uint idTexCoords = 0; 
	float* texCoords = nullptr;

	AABB enclosingBox;
};

struct State 
{
	
	float gravity = -9.8f;
	float size = 0.0f;
	float4 color = float4::one;
};

class ParticleSystem;

class Particle
{
	
public:

	Particle(ParticleSystem* parent, const SystemState& initial , const SystemState& final, float3 direction, float MaxLifeTime);
	~Particle();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	void SetState(State& myState,const SystemState& sState);
	void CalcInterpolation();

	void DrawParticle();

private:
	ParticleSystem * pSystem;
	ParticleData data;
	State Initial;
	State Final;
	bool killThis = false;

};





#endif // !_PS_PARTICLE_
