#pragma once

#include "Globals.h"
#include "Module.h"
#include "Timer.h"

#include "Resource.h"
#include <map>
#include <vector>

class aiMesh;
class Timer;

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool startEnabled = true);
	~ModuleResources();

	update_status PostUpdate(float dt);

	int Find(const char* fileName);
	int ImportFile(const char* fileName, ResourceType type = Resource_Unknown);
	int ImportFile(const char* meshName, aiMesh* mesh);
	Resource* Get(int UID);
	Resource* CreateNewResource(ResourceType type, int UID);
	std::vector<Resource*> GetResourcesOfType(ResourceType type) const;

	void CheckTextures();
	void ReImportFile(const char* fileName);

	void SaveResources(Configuration& resources) const;
	void LoadResources(Configuration& resources);

private:
	std::map<int, Resource*> resources;

	Timer checkTexturesTimer;
};