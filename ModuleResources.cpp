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

int ModuleResources::ImportFile(const char * fileName, ResourceType type)
{
	//Check that the file isn't already loaded
	int UID = Find(fileName);

	if (UID == 0)
	{
		bool imported;
		UID = App->randomGenerator->Int();
		std::string exportedFile = std::to_string(UID);

		switch(type)
		{
		case Resource_Texture:
		{
			imported = App->textures->Import(fileName, exportedFile);
			break;
		}
		case Resource_Unknown:
		{
			LOG("Trying to import a resource with unknown format!");
			return -1;
		}
		}

		if (imported == true)
		{
			Resource* newResource = CreateNewResource(type, UID);
			newResource->file = fileName;
			newResource->exportedFile = "Library/Material/";
			newResource->exportedFile += exportedFile;
			newResource->exportedFile += ".dds";
			return UID;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return UID;
	}
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
