#ifndef _PS_PARTICLE_SYSTEM_
#define _PS_PARTICLE_SYSTEM_

#include "../Application.h"
#include <vector>
#include "../MathGeo/MathGeoLib.h"
#include "GlobalDefines.h"
#include "../ModuleTextures.h"






// -------------------------------------- [STATE]
struct SystemState //going to global state to set on particles latter
{	
	float size = 1.0f;
	float gravity = 9.8f;
	float gravityVariation = 0.0f;
	float speed = 1.0f;
	float4 color = float4::one; 
	float4 color2 = float4::one;
};
// -------------------------------------- [TEXTURE]
struct TextureData // it is like resource texture, contains all the data of the texture.
{
	
	uint textureID = 0;//App->textures->ImportImage("../Game/Assets/Baker_house.png");
	float4 color = float4::one;
};

struct PTransformation
{
	float3 position;
	Quat rotation;
	float3 scale;
};

// -------------------------------------- [PARTICLE SYSTEM]
class Particle;
class Emiter;
struct ParticleMesh;


class ParticleSystem
{
	friend class Particle;	

public:
	ParticleSystem();
	~ParticleSystem();
	
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	//UI -------------------------------
	
	//Draws ---
	void DrawParticleSystemEditor();
	void Draw();

	
	void DrawBasicEditor();
	
	//Show/unshow Window ---
	void openCloseWindowPS(bool state);
	bool isOpenCloseWindowPS();
	void changeOpenCloseWindowPS();
	
	void SetTransform(float3 Position, Quat rotation, float3 scale);

	ParticleMesh * GetMesh() const;

	void SetPlaneMesh();

	void CreateParticle();


private:
	ParticleMesh* particleMesh;
	std::vector<Particle*> particleVec;
	Emiter* emiter;
	SystemState initialState;
	SystemState finalState;
	bool windowShow = true;

private: //times
	float timeToCreateNParticle = 0.f;

public:	
	PTransformation* transformation; 
	TextureData tData;
	float3 cameraPos = float3::zero;

};

#endif // !_PS_PARTICLE_SYSTEM_
