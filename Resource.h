#pragma once

#include "Globals.h"
#include <string>

class Configuration;

enum ResourceType
{
	Resource_Texture,
	Resource_Mesh,
	Resource_Unknown
};

class Resource
{
	friend class ModuleResources;
public:
	Resource(int UID, ResourceType type);
	virtual ~Resource();

	ResourceType GetType()const;
	int GetUID()const;
	const char* GetFile()const;
	const char* GetExportedFile()const;
	bool IsLoadedToMemory()const;
	void LoadToComponent();
	void UnloadFromComponent();
	uint GetReferenceCount()const;

	virtual void Save(Configuration& dataToSave) const;
	virtual void Load(Configuration& dataToLoad);
	virtual bool LoadInMemory() = 0;
	virtual bool UnloadFromMemory() = 0;

protected:
	int UID = 0;
	std::string file;
	std::string exportedFile;

	ResourceType type = Resource_Unknown;
	uint references = 0;
};