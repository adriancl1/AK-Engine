#ifndef _PS_PARTICLE_SYSTEM_
#define _PS_PARTICLE_SYSTEM_

#include <vector>


typedef unsigned int uint;

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

};
struct SystemState //going to global state to set on particles latter
{


};

struct TextureData // it is like resource texture, contains all the data of the texture.
{


};

#endif // !_PS_PARTICLE_SYSTEM_
