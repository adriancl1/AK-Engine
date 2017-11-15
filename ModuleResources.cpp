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

int ModuleResources::ImportFile(const char* meshName, aiMesh * mesh)
{
	int UID = Find(meshName);

	if (UID == 0)
	{
		bool imported;
		int UID = App->randomGenerator->Int();
		std::string exFile = std::to_string(UID);

		imported = App->importer->SaveOwnFormat(mesh, exFile.c_str());

		if (imported == true)
		{
			Resource* newResource = CreateNewResource(Resource_Mesh, UID);
			newResource->file = meshName;
			newResource->exportedFile = "Library/Mesh/";
			newResource->exportedFile += exFile;
			newResource->exportedFile += ".don";
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

std::vector<Resource*> ModuleResources::GetResourcesOfType(ResourceType type) const
{
	 std::vector<Resource*> ret;
	 for (std::map<int, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	 {
		 if ((*it).second->GetType() == type)
		 {
			 ret.push_back((*it).second);
		 }
	 }
	 return ret;
}

void ModuleResources::SaveResources(Configuration& save) const
{
	for (std::map<int, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		(*it).second->Save(save);
	}
}

void ModuleResources::LoadResources(Configuration& resources)
{
	for (int i = 0; i < resources.GetArraySize("Scene Resources"); i++)
	{
		Configuration tmpConfig = resources.GetArray("Scene Resources", i);
		int tmpUID = tmpConfig.GetInt("UID");
		if (Get(tmpUID) == nullptr)
		{
			switch(tmpConfig.GetInt("Type"))
			{
			case Resource_Mesh:
			{
				ResourceMesh* resourceMesh = (ResourceMesh*)CreateNewResource(Resource_Mesh, tmpUID);
				resourceMesh->Load(tmpConfig);
				break;
			}
			case Resource_Texture:
			{
				ResourceTexture* resourceTexture = (ResourceTexture*)CreateNewResource(Resource_Texture, tmpUID);
				resourceTexture->Load(tmpConfig);
				break;
			}
			}
		}
	}
}


