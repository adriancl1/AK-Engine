#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "imgui-1.51\imgui.h"
#include "imgui-1.51\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{}

// Load assets
bool ModuleImGui::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->GetWindow());

	return ret;
}

// PreUpdate
update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());
	return(UPDATE_CONTINUE);
}

// Update
update_status ModuleImGui::Update(float dt)
{
	if (ImGui::Button("Open Demo"))
	{
		testWindowActive = true;
	}
	else if (ImGui::Button("Close Demo"))
	{
		testWindowActive = false;
	}
	if (ImGui::Button("Close App"))
	{
		return UPDATE_STOP;
	}

	if (testWindowActive)
	{
		ImGui::ShowTestWindow();
	}
	
	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	LOG("Unloading Intro scene");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}
