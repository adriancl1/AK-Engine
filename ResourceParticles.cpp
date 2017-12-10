#include "ResourceParticles.h"

ResourceParticles::ResourceParticles(int UID) : Resource(UID, Resource_Particle)
{

}

ResourceParticles::~ResourceParticles()
{
}

bool ResourceParticles::LoadInMemory()
{
	return true;
}

bool ResourceParticles::UnloadFromMemory()
{
	return true;
}
