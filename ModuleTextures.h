#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include "Module.h"
#include "Globals.h"


class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Init(Configuration data);
	bool CleanUp(Configuration data);
	bool Import(const char* fileDir, std::string& output_file);

	uint ImportImage(const char * image);
};

#endif 