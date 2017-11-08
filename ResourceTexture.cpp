#include "ResourceTexture.h"
#include "Application.h"
#include "ModuleTextures.h"

ResourceTexture::ResourceTexture(int UID): Resource(UID, Resource_Texture)
{
}

ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::LoadInMemory()
{
	textureID = App->textures->ImportImage(exportedFile.c_str());
	if (textureID == 0)
	{
		return false;
	}
	return true;
}

bool ResourceTexture::UnloadFromMemory()
{
	App->textures->DeleteImage(textureID);
	textureID = 0;
	return true;
}

void ResourceTexture::Save(Configuration & config) const
{
}

void ResourceTexture::Load(Configuration & config)
{
}
