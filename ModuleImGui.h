#ifndef __MODULEIMGUI_H__
#define __MODULEIMGUI_H__

#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui-1.51\imgui.h"

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
	bool menuActive = false;
	bool closeApp = false;

//ImGui
public:
	IMGUI_API void ShowMenuWindow(bool* p_open = NULL);
	IMGUI_API void ShowDebugWindow(bool* p_open = NULL);
};

#endif // __MODULEIMGUI_H__