#pragma once

#include "Resource.h"

class ResourceTexture : public Resource
{
public:
	ResourceTexture(int UID);
	~ResourceTexture();

	bool LoadInMemory()override;
	bool UnloadFromMemory()override;
	void Save(Configuration& config)const override;
	void Load(Configuration& config)override;

public:
	uint textureID = 0;
};