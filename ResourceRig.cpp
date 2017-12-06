#include "ResourceRig.h"

ResourceRig::ResourceRig(int UID) : Resource(UID, Resource_Mesh)
{
}

ResourceRig::~ResourceRig()
{
}

bool ResourceRig::LoadInMemory()
{
	return false;
}

bool ResourceRig::UnloadFromMemory()
{
	return false;
}
