#pragma once

#include "Globals.h"
#include "Module.h"

#include "Resource.h"
#include <map>

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool startEnabled = true);
	~ModuleResources();
	int Find(const char* fileName);
	int ImportFile(const char* fileName);
	Resource* Get(int UID);
	Resource* CreateNewResource(ResourceType type, int UID);

private:
	std::map<int, Resource*> resources;
};