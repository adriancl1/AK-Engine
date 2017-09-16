#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "imgui-1.51\imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include <stdio.h>


using namespace std;

ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleImGui::~ModuleImGui()
{
}

// Called before render is available
bool ModuleImGui::Init()
{
	LOG("Init editor gui with imgui lib version %s", ImGui::GetVersion());

	ImGui_ImplSdlGL3_Init(App->window->GetWindow());

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "Game/imgui.ini";

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

// Called before quitting
bool ModuleImGui::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();

	return true;
}