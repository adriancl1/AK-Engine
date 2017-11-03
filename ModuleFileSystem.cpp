#include "ModuleFileSystem.h"

#include <fstream>

#define MESH_DIRECTORY "Library/Mesh"
#define MESH_EXTENSION ".don"
#define MATERIAL_DIRECTORY "Library/Material"
#define MATERIAL_EXTENSION ".dds" 
#define SCENE_DIRECTORY "Library/Scenes"
#define SCENE_EXTENSION ".akS"

ModuleFileSystem::ModuleFileSystem(Application * app, bool startEnabled) : Module(app, startEnabled)
{
	name = "file system";
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Init(Configuration data)
{
	CreateNewDirectory("Library");
	CreateNewDirectory(MESH_DIRECTORY);
	CreateNewDirectory(MATERIAL_DIRECTORY);
	CreateNewDirectory(SCENE_DIRECTORY);

	return true;
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
	else if (type == fileScene)
	{
		path += SCENE_DIRECTORY;
		path += "/";
		path += name;
		path += SCENE_EXTENSION;
	}

	std::ofstream file(path.c_str(), std::ofstream::out | std::ofstream::binary);
	file.write(buffer, bufferSize);
	file.close();
}

bool ModuleFileSystem::LoadFile(const char * name, char ** buffer, int& size, FileType type)
{
	bool ret = false;

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
	else if (type == fileScene)
	{
		path += SCENE_DIRECTORY;
		path += "/";
		path += name;
		path += SCENE_EXTENSION;
	}

	std::ifstream file(path, std::ifstream::binary);

	if (file.good())
	{
		if (file.is_open())
		{
			file.seekg(0, file.end);
			std::streamsize length = file.tellg();
			file.seekg(0, file.beg);

			*buffer = new char[length];

			file.read(*buffer, length);

			if (file)
			{
				LOG("File %s loaded succesfully", name);
				ret = true;
			}
			else
			{
				LOG("Couldn't load %s", name);
			}
			file.close();
		}
	}
	return ret;
}
