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
	

private:
	std::vector<Particle*> particleVec;
	Emiter* emiter;

public:

	float3 cameraPos = float3::zero;

};
struct SystemState //going to global state to set on particles latter
{


};

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
