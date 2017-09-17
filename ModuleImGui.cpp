#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "imgui-1.51\imgui.h"
#include "imgui-1.51\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

//Displays usefull information about that option
static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

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
	ShowDebugWindow();

	if (menuActive)
	{
		ShowMenuWindow();
	}
	if (testWindowActive)
	{
		ImGui::ShowTestWindow();
	}
	if (closeApp)
	{
		return UPDATE_STOP;
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

void ModuleImGui::ShowDebugWindow(bool* p_open)
{
	static bool openMenuWindow = false;

	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Debug", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	if (ImGui::CollapsingHeader("Debug Options"))
	{
		if (ImGui::Checkbox("View Menu", &openMenuWindow))
		{
			menuActive = !menuActive;
		}
	}
	ImGui::End();
}

void ModuleImGui::ShowMenuWindow(bool* p_open)
{
	static bool openTestWindow = false;
	static bool viewGrid = false;

	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("ImGui Menu", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	ImGui::Text("ImGui Menu");

	if (ImGui::CollapsingHeader("Options"))
	{
		ImGui::TextWrapped("Options Menu");

		if (ImGui::Checkbox("View Demo", &openTestWindow))
		{
			testWindowActive = !testWindowActive;
		}
	
		if (ImGui::TreeNode("Graphic Options"))
		{
			ImGui::Text("Not functional");
			if (ImGui::Checkbox("View ---", &viewGrid))
			{
			}

			ImGui::TreePop();
		}
	}

	if (ImGui::Button("Close App", ImVec2(300,20)))
	{
		closeApp = true;
	}
	ImGui::End();
}
