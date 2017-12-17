#include "Globals.h"
#include "Application.h"
#include "Brofiler-1.1.2/Brofiler.h"
#include "Math.h"
#include "ModuleImGui.h"
#include "imgui-1.51/imgui.h"
#include "imgui-1.51/imgui_impl_sdl_gl3.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Glew/include/glew.h"


#pragma comment( lib, "Glew/libx86/glew32.lib" )

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
	name = "imgui";
}

ModuleImGui::~ModuleImGui()
{
}

//Load assets
bool ModuleImGui::Start()
{
	BROFILER_CATEGORY("Module ImGui Start", Profiler::Color::AliceBlue);

	LOG("Loading Intro assets");
	bool ret = true;

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->GetWindow());

	//ImGui Menu Active Booleans
	consoleActive = false;
	configurationActive = false;
	aboutActive = false;
	editorActive = true;

	openConsoleWindow = false;
	openConfigurationWindow = false;
	openAboutWindow = false;
	openEditorWindow = true;

	return ret;
}

//PreUpdate
update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());
	ImGuizmo::BeginFrame();
	return(UPDATE_CONTINUE);
}

//Update
update_status ModuleImGui::Update(float dt)
{
	BROFILER_CATEGORY("Module ImGui Update", Profiler::Color::AliceBlue);

	//Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save Scene"))
			{
				App->sceneEditor->SaveScene("AKScene");
			}
			if (ImGui::MenuItem("Load Scene"))
			{
				App->sceneEditor->WantToLoadScene("Assets/Scenes/AKScene.akS");
			}
			if (ImGui::MenuItem("Quit", "ESC"))
			{
				return UPDATE_STOP;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console"))
			{
				openConsoleWindow = !openConsoleWindow;
				consoleActive = !consoleActive;
			}
			if (ImGui::MenuItem("Configuration"))
			{
				openConfigurationWindow = !openConfigurationWindow;
				configurationActive = !configurationActive;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Empty Game Object"))
			{
				App->sceneEditor->CreateEmptyGameObject();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/blob/master/README.md", NULL, NULL, SW_SHOWNORMAL); // This should be the wiki when we have one.
			}
			if (ImGui::MenuItem("Latest Release"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/releases", NULL, NULL, SW_SHOWNORMAL); 
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/issues", NULL, NULL, SW_SHOWNORMAL); 
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (consoleActive)
	{
		ShowConsoleWindow();
	}
	if (configurationActive)
	{
		ShowConfigurationWindow();
	}
	if (editorActive)
	{
		ShowEditorWindow();
	}
	if (aboutActive)
	{
		ShowAboutWindow();
	}

	if (closeApp)
	{
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp(Configuration data)
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleImGui::ShowDebugWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Debug", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	if (ImGui::CollapsingHeader("Debug Options"))
	{
		if (ImGui::Checkbox("Show Console", &openConsoleWindow))
		{
			consoleActive = !consoleActive;
		}
		if (ImGui::Checkbox("Show Configuration", &openConfigurationWindow))
		{
			configurationActive = !configurationActive;
		}
		if (ImGui::Checkbox("Show Editor", &openEditorWindow))
		{
			editorActive = !editorActive;
		}
		if (ImGui::Checkbox("Show About..", &openAboutWindow))
		{
			aboutActive = !aboutActive;
		}
	}
	ImGui::End();
}

void ModuleImGui::Draw()const
{
	ImGui::Render();
}

void ModuleImGui::ShowConsoleWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Console", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	if (ImGui::Button("Clear"))
	{
		consoleText.clear();
	}

	for (int i = consoleText.size() - 1; i >= 0; i--)
	{
		ImGui::Text("%s", consoleText[i].c_str());
	}

	ImGui::End();
}

void ModuleImGui::AddLogToWindow(std::string toAdd)
{
	consoleText.push_back(toAdd);
}

void ModuleImGui::ProcessEvent(SDL_Event * event)
{
	if (event != nullptr)
	{
		ImGui_ImplSdlGL3_ProcessEvent(event);
	}
}

void ModuleImGui::ShowConfigurationWindow(bool* p_open)
{
	if (!ImGui::Begin("Configuration", p_open))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	ImGui::Text("Options");

	App->OnConfiguration();

	ImGui::End();
}

void ModuleImGui::ShowAboutWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("About", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

										
	ImGui::Text("Name: AK Engine.");
	ImGui::Text("3D Engine made with C++ and OpenGL for an assignment in a Game Design & Development degree.");
	ImGui::Text("Authors: Marc Fabian, Adrian Castillo and Marc Lopez.");
	
	if (ImGui::CollapsingHeader("Libraries used"))
	{
		if (ImGui::MenuItem("SDL 2.0.4"))
		{
			ShellExecuteA(NULL, "open", "https://www.libsdl.org/index.php", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("MathGeoLib 1.3"))
		{
			ShellExecuteA(NULL, "open", "http://clb.demon.fi/MathGeoLib/nightly/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("ImGui 1.52 WIP"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("Bullet 2.84"))
		{
			ShellExecuteA(NULL, "open", "http://bulletphysics.org/wordpress/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("Brofiler 1.1.2"))
		{
			ShellExecuteA(NULL, "open", "http://brofiler.com/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("Glew 2.0.0"))
		{
			ShellExecuteA(NULL, "open", "http://glew.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("OpenGl 3.1"))
		{
			ShellExecuteA(NULL, "open", "https://www.opengl.org/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("DevIL 1.8.0"))
		{
			ShellExecuteA(NULL, "open", "http://openil.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("Assimp 3.0"))
		{
			ShellExecuteA(NULL, "open", "http://assimp.sourceforge.net/lib_html/", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	if (ImGui::CollapsingHeader("License"))
	{
		if (ImGui::MenuItem("License Apache 2.0"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/blob/master/LICENSE", NULL, NULL, SW_SHOWNORMAL);
		}
	}


	ImGui::End();
}

void ModuleImGui::ShowEditorWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Show Editor", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	App->sceneEditor->ShowEditor();

	ImGui::End();
}