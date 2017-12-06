#ifndef __ModuleFileSystem_H__
#define __ModuleFileSystem_H__

#include "Globals.h"
#include "Module.h"

enum FileType
{
	fileMesh,
	fileMaterial,
	fileScene,
	fileTexture,
	fileRig
};

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool startEnabled = true);
	~ModuleFileSystem();

	bool Init(Configuration data);

	bool CreateNewDirectory(const char* name);
	void SaveFile(const char* name, char* buffer, int bufferSize, FileType type);
	bool LoadFile(const char* name, char** buffer, uint& size, FileType type);
};

#endif
