#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include "Module.h"
#include "Globals.h"


class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Init(JSON_Object* data = nullptr);
	bool CleanUp(JSON_Object* data = nullptr);
	bool Import(std::string& output_file);

	uint ImportImage(const char * image);
};

#endif 