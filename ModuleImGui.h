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
	bool testWindowActive = false;
	bool menuActive = false;
	bool consoleActive = false;
	bool mathPlaygroundActive = false;
	bool configurationActive = false;
	bool aboutActive = false;
	bool createGeometryActive = false;

	bool closeApp = false;

	std::vector<std::string> consoleText;
	std::vector<float> FPSData;
	std::vector<float> MsData;

	char title[128] = "AK Engine";
	bool fullscreen;
	bool fullDesktop;
	bool borderless;
	int windowWidth;
	int windowHeight;
	int volume;
	float brightness;

	bool wireframe;

//ImGui
public:
	IMGUI_API void ShowMenuWindow(bool* p_open = NULL);
	IMGUI_API void ShowDebugWindow(bool* p_open = NULL);
	IMGUI_API void ShowConsoleWindow(bool* p_open = NULL);
	IMGUI_API void ShowMathWindow(bool* p_open = NULL);
	IMGUI_API void ShowConfigurationWindow(bool* p_open = NULL);
	IMGUI_API void ShowAboutWindow(bool* p_open = NULL);
	IMGUI_API void ShowCreateGeometryWindow(bool* p_open = NULL);
	void AddLogToWindow(std::string toAdd);

private:
	void CycleFPSAndMsData(float fps, float ms);

private:
	//Booleans for ImGui Checkbox buttons
	bool openMenuWindow;
	bool openConsoleWindow;
	bool openConfigurationWindow;
	bool openMathPlaygroundWindow;
	bool openAboutWindow;
	bool openCreateGeometryWindow;

//Math Geometric Shape
public:
	//Properties Sphere 1
	int sphereRadius = 0;
	int sphereX = 0;
	int sphereY = 0;
	int sphereZ = 0;

	//Sphere 2
	int sphereRadius2 = 0;
	int sphereX2 = 0;
	int sphereY2 = 0;
	int sphereZ2 = 0;

	//Capsule 1
	int capsuleRadius = 0;
	int capsuleBotX = 0;
	int capsuleBotY = 0;
	int capsuleBotZ = 0;
	int capsuleTopX = 0;
	int capsuleTopY = 0;
	int capsuleTopZ = 0;

	//Booleans
	bool intersects = false;
	bool intersectsTrue = false;
	bool intersectsFalse = false;
};

#endif // __ModuleImGui_H__