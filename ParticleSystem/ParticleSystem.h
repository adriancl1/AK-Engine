#ifndef _PS_PARTICLE_SYSTEM_
#define _PS_PARTICLE_SYSTEM_

#include <vector>
#include "../MathGeo/MathGeoLib.h"
#include "GlobalDefines.h"



// -------------------------------------- [STATE]
struct SystemState //going to global state to set on particles latter
{	
	float size = 1.0f;
	float gravity = 9.8f;
	float speed = 1.0f;
	float4 color = float4::one; 
	float4 color2 = float4::one;
};
// -------------------------------------- [TEXTURE]
struct TextureData // it is like resource texture, contains all the data of the texture.
{
	uint textureID = 0;
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

class ParticleSystem
{
	friend class Particle;
public:
	ParticleSystem();
	~ParticleSystem();
	
	//UI -------------------------------
	
	//Draws ---
	void DrawParticleSystemEditor();
	void Draw();
	
	void DrawBasicEditor();
	
	//Show/unshow Window ---
	void openCloseWindowPS(bool state);
	bool isOpenCloseWindowPS();
	void changeOpenCloseWindowPS();
	

private:
	std::vector<Particle*> particleVec;
	Emiter* emiter;

	SystemState initialState;
	SystemState finalState;

	bool windowShow = true;

public:

	float3 cameraPos = float3::zero;

};

#endif // !_PS_PARTICLE_SYSTEM_
