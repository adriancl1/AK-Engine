#ifndef __ModuleImGui_H__
#define __ModuleImGui_H__

#include "Module.h"
#include "Globals.h"
#include "imgui-1.51\imgui.h"
#include <string>
#include <vector>

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
	bool consoleActive;
	bool configurationActive;
	bool aboutActive;
	bool editorActive;

	bool closeApp = false;

	//Used to print Logs into the console window
	std::vector<std::string> consoleText;

public:
	//ImGui Windows
	IMGUI_API void TopMenu();
	IMGUI_API void ShowConsoleWindow(bool* p_open = NULL);
	IMGUI_API void ShowConfigurationWindow(bool* p_open = NULL);
	IMGUI_API void ShowAboutWindow(bool* p_open = NULL);
	IMGUI_API void ShowEditorWindow(bool* p_open = NULL);

	//Console Logs
	void AddLogToWindow(std::string toAdd);

private:
	//Booleans for ImGui Checkbox buttons
	bool openConsoleWindow;
	bool openConfigurationWindow;
	bool openAboutWindow;
	bool openEditorWindow;

};

#endif // __ModuleImGui_H__