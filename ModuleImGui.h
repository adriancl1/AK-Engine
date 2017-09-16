#ifndef __MODULEIMGUI_H__
#define __MODULEIMGUI_H__

#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleImGui : public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

private:
	bool testWindowActive = false;
};

#endif // __MODULEIMGUI_H__