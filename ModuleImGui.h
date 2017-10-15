#ifndef __ModuleImGui_H__
#define __ModuleImGui_H__

#include "Module.h"
#include "Globals.h"
#include "imgui-1.51\imgui.h"
#include <string>
#include <vector>

#define IM_ARRAYSIZE(_ARR)      ((int)(sizeof(_ARR)/sizeof(*_ARR)))

class ModuleImGui : public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp(JSON_Object* data = nullptr);

	void Draw() const;

private:
	//ImGui Menu Active Booleans
	bool menuActive;
	bool UIBottom;
	bool configurationActive;
	bool aboutActive;
	bool UIRight;

	bool closeApp = false;

	std::vector<std::string> consoleText;

//ImGui
public:
	IMGUI_API void TopMenu();
	IMGUI_API void ShowBottomUI(bool* p_open = NULL);
	IMGUI_API void ShowRightUI(bool* p_open = NULL);

	void AddLogToWindow(std::string toAdd);
};

#endif // __ModuleImGui_H__