#pragma once

#include "Resource.h"

class ResourceParticles : public Resource
{
public:
	ResourceParticles(int UID);
	~ResourceParticles();

	bool LoadInMemory()override;
	bool UnloadFromMemory()override;

public:
	uint textureID = 0;
};