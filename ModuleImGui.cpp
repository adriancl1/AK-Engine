#include "Globals.h"
#include "Application.h"
#include "Math.h"
#include "ModuleImGui.h"

#include "Brofiler-1.1.2\Brofiler.h"
#include "imgui-1.51\imgui.h"
#include "imgui-1.51\imgui_dock.h"
#include "imgui-1.51\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"

#include "mmgr/mmgr.h"

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
	UIBottom = true;
	UIRight = true;

	//IMGUI STYLE START
	ImGuiStyle& UIstyle = ImGui::GetStyle();

	UIstyle.WindowPadding = ImVec2(10, 15);
	UIstyle.WindowRounding = 2.0f;
	UIstyle.FramePadding = ImVec2(5, 5);
	UIstyle.FrameRounding = 2.0f;
	UIstyle.ItemSpacing = ImVec2(4, 4);
	UIstyle.ItemInnerSpacing = ImVec2(4, 4);
	UIstyle.IndentSpacing = 25.0f;
	UIstyle.ScrollbarSize = 15.0f;
	UIstyle.ScrollbarRounding = 2.0f;
	UIstyle.GrabMinSize = 5.0f;
	UIstyle.GrabRounding = 2.0f;

	UIstyle.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	UIstyle.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	UIstyle.Colors[ImGuiCol_WindowBg] = ImVec4(0.8f, 0.8f, 0.8f, 1.00f);
	UIstyle.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.7f, 0.7f, 0.7f, 0.80f);
	UIstyle.Colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	UIstyle.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	UIstyle.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	UIstyle.Colors[ImGuiCol_FrameBg] = ImVec4(0.4f, 0.5f, 0.6f, 1.00f);
	UIstyle.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.5f, 0.6f, 0.7f, 0.80f);
	UIstyle.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.7f, 0.8f, 0.9f, 0.40f);
	UIstyle.Colors[ImGuiCol_TitleBg] = ImVec4(0.47f, 0.47, 0.47f, 1.00f);
	UIstyle.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.47f, 0.47, 0.47f, 0.75f);
	UIstyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.47f, 0.47, 0.47f, 1.00f);
	UIstyle.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.8f, 0.8f, 0.8f, 1.00f);
	UIstyle.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	UIstyle.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	UIstyle.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.5f, 0.6f, 0.7f, 0.80f);
	UIstyle.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.7f, 0.8f, 0.9f, 0.40f);
	UIstyle.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	UIstyle.Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.46f, 0.0f, 1.00f);
	UIstyle.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	UIstyle.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7f, 0.8f, 0.9f, 0.40f);
	UIstyle.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.5f, 0.6f, 1.00f);
	UIstyle.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.6f, 0.7f, 0.80f);
	UIstyle.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7f, 0.8f, 0.9f, 0.40f);
	UIstyle.Colors[ImGuiCol_Header] = ImVec4(0.4f, 0.5f, 0.6f, 1.00f);
	UIstyle.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.5f, 0.6f, 0.7f, 0.80f);
	UIstyle.Colors[ImGuiCol_HeaderActive] = ImVec4(0.7f, 0.8f, 0.9f, 0.40f);
	UIstyle.Colors[ImGuiCol_Column] = ImVec4(0.4f, 0.5f, 0.6f, 1.00f);
	UIstyle.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.5f, 0.6f, 0.7f, 0.80f);
	UIstyle.Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	UIstyle.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	UIstyle.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.5f, 0.6f, 0.7f, 0.80f);
	UIstyle.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.7f, 0.8f, 0.9f, 0.40f);
	UIstyle.Colors[ImGuiCol_CloseButton] = ImVec4(0.4f, 0.5f, 0.6f, 1.00f);
	UIstyle.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.5f, 0.6f, 0.7f, 0.80f);
	UIstyle.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	UIstyle.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	UIstyle.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	UIstyle.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	UIstyle.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	UIstyle.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	UIstyle.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	ImGuiIO& io{ ImGui::GetIO() };

	return ret;
}

//PreUpdate
update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());
	return(UPDATE_CONTINUE);
}

//Update
update_status ModuleImGui::Update(float dt)
{
	BROFILER_CATEGORY("Module ImGui Update", Profiler::Color::AliceBlue);

	TopMenu();

	if (UIBottom)
	{
		ShowBottomUI();
	}
	if (UIRight)
	{
		ShowRightUI();
	}

	if (closeApp)
	{
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp(JSON_Object* data)
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleImGui::Draw()const
{
	ImGui::Render();
}

void ModuleImGui::TopMenu()
{
	//Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "ESC"))
			{
				closeApp = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Bottom UI"))
			{
				UIBottom = !UIBottom;
			}
			if (ImGui::MenuItem("Right UI"))
			{
				UIRight = !UIRight;
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
}

void ModuleImGui::ShowBottomUI(bool* p_open)
{
	
}

void ModuleImGui::ShowRightUI(bool* p_open)
{
	ImGui::Begin("RightUI", NULL, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

	ImGui::BeginDockspace();

	if (ImGui::BeginDock("Editor", false, false, false)) 
	{
		App->sceneEditor->ShowEditor();
	}

	ImGui::EndDock();

	if (ImGui::BeginDock("Configuration", false, false, false))
	{
		App->OnConfiguration();
	}

	ImGui::EndDock();

	ImGui::Begin("InferiorUI", NULL, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (ImGui::BeginDock("Console", false, false, true))
	{
		for (int i = consoleText.size() - 1; i >= 0; i--)
		{
			ImGui::Text("%s", consoleText[i].c_str());
		}
		if (ImGui::Button("Clear"))
		{
			consoleText.clear();
		}
	}

	ImGui::EndDock();

	if (ImGui::BeginDock("About", false, false, false))
	{
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
			if (ImGui::MenuItem("License Apache 2.0"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/blob/master/LICENSE", NULL, NULL, SW_SHOWNORMAL);
			}
		}
	}
	ImGui::EndDock();

	ImGui::EndDockspace();

	ImGui::End();
	ImGui::End();
}

void ModuleImGui::AddLogToWindow(std::string toAdd)
{
	consoleText.push_back(toAdd);
}
