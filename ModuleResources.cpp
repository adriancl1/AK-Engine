#include "ModuleResources.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ResourceTexture.h"

ModuleResources::ModuleResources(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	name = "resources";
}

ModuleResources::~ModuleResources()
{
}

int ModuleResources::Find(const char * fileName)
{
	for (std::map<int, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (strcmp(it->second->GetFile(), fileName) == 0)
		{
			return it->first;
		}
	}
	return 0;
}

int ModuleResources::ImportFile(const char * fileName)
{
	//Check that the file isn't already loaded
	if (Find(fileName) == 0)
	{
		int length = strlen(fileName);
		bool imported;
		int UID = App->randomGenerator->Int();
		std::string exportedFile = std::to_string(UID);
		ResourceType type = Resource_Unknown;

		if (strcmp(&fileName[length - 4], ".png") == 0 || strcmp(&fileName[length - 4], ".PNG") == 0)
		{
			imported = App->textures->Import(fileName, exportedFile);
			type = Resource_Texture;
		}

		if (imported == true)
		{
			Resource* newResource = CreateNewResource(type, UID);
			newResource->file = fileName;
			newResource->exportedFile = exportedFile;
			return UID;
		}
	}
	return 0;
}

Resource * ModuleResources::Get(int UID)
{
	std::map<int, Resource*>::iterator it = resources.find(UID);
	if (it != resources.end())
		return it->second;
	return nullptr;
}

Resource * ModuleResources::CreateNewResource(ResourceType type, int UID)
{
	Resource* ret = nullptr;

	switch (type)
	{
	case Resource_Texture:
	{
		ret = (Resource*) new ResourceTexture(UID);
	}
	}

	if (ret != nullptr)
	{
		resources[UID] = ret; //Put it on the map
	}

	return ret;
}
