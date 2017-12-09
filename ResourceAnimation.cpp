#include "ResourceAnimation.h"
#include "Application.h"

ResourceAnimation::ResourceAnimation(int UID) : Resource(UID, Resource_Animation)
{
}

ResourceAnimation::~ResourceAnimation()
{
}

bool ResourceAnimation::LoadInMemory()
{
	App->importer->LoadAnimOwnFormat(exportedFile.c_str(), this);
	return true;
}

bool ResourceAnimation::UnloadFromMemory()
{
	return true;
}
