#ifndef _PS_PARTICLE_SYSTEM_
#define _PS_PARTICLE_SYSTEM_

#include <vector>
#include "../MathGeo/MathGeoLib.h"
#include "GlobalDefines.h"


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
	//Show/unshow Window ---
	void openCloseWindowPS(bool state);
	bool isOpenCloseWindowPS();
	void changeOpenCloseWindowPS();
	
private:
	std::vector<Particle*> particleVec;
	Emiter* emiter;

	bool windowShow = true;

public:

	float3 cameraPos = float3::zero;

};


// -------------------------------------- [STATE]
struct SystemState //going to global state to set on particles latter
{


};
// -------------------------------------- [TEXTURE]
struct TextureData // it is like resource texture, contains all the data of the texture.
{


};

struct PTransformation
{
	float3 Position;
	Quat Rotation;
	float3 Scale;
};



#endif // !_PS_PARTICLE_SYSTEM_
