#include "ModuleResources.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "ModuleImporter.h"

#include "Assimp\include\cimport.h" 
#include "Assimp\include\scene.h" 
#include "Assimp\include\postprocess.h" 
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

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
		std::string exFile = std::to_string(UID);

		switch(type)
		{
		case Resource_Texture:
		{
			imported = App->textures->Import(fileName, exFile);
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
			newResource->exportedFile += exFile;
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

int ModuleResources::ImportFile(aiMesh * mesh)
{
	//TODO: Assign or read a name from aiMesh
	bool imported;
	int UID = App->randomGenerator->Int();
	std::string exFile = std::to_string(UID);

	imported = App->importer->SaveOwnFormat(mesh, exFile.c_str());

	if (imported == true)
	{
		Resource* newResource = CreateNewResource(Resource_Mesh, UID);
		//newResource->file = fileName; ?

		newResource->exportedFile = "Library/Mesh/";
		newResource->exportedFile += exFile;
		newResource->exportedFile += ".don";
		return UID;
	}
	else
	{
		return -1;
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
		break;
	}
	case Resource_Mesh:
	{
		ret = (Resource*) new ResourceMesh(UID);
		break;
	}
	}

	if (ret != nullptr)
	{
		resources[UID] = ret; //Put it on the map
	}

	return ret;
}
