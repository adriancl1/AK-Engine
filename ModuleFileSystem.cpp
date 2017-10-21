#include "ModuleFileSystem.h"

#include <fstream>

#define MESH_DIRECTORY "Library/Mesh"
#define MESH_EXTENSION ".don"
#define MATERIAL_DIRECTORY "Library/Material"
#define MATERIAL_EXTENSION ".dds" 

ModuleFileSystem::ModuleFileSystem(Application * app, bool startEnabled) : Module(app, startEnabled)
{
	name = "file system";
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::CreateNewDirectory(const char * name)
{
	if (CreateDirectory(name, NULL))
	{
		LOG("Created %s folder.", name);
		return true;
	}
	else
	{
		LOG("Couldn't create %s folder.", name);
		return false;
	}
}

void ModuleFileSystem::SaveFile(const char * name, char * buffer, int bufferSize, FileType type)
{
	std::string path; 
	if (type == fileMesh)
	{
		path = MESH_DIRECTORY;
		path += "/";
		path += name;
		path += MESH_EXTENSION;
	}
	else if (type == fileMaterial)
	{
		path = MATERIAL_DIRECTORY;
		path += "/";
		path += name;
		path += MATERIAL_EXTENSION;
	}

	std::ofstream file(path.c_str(), std::ofstream::out | std::ofstream::binary);
	file.write(buffer, bufferSize);
	file.close();
}

bool ModuleFileSystem::LoadFile(const char * name, char * buffer, int& size)
{
	std::ifstream file(name, std::ifstream::in | std::ifstream::binary);
	size = file.gcount();
	
	if (file.read(buffer, size))
	{
		LOG("Couldn't read %s.", name);
		return false;
	}
	else
	{
		LOG("File %s read succesfully.", name);
		return true;
	}
}
