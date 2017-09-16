#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "imgui-1.51\imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include "imgui-1.51\gl3w\GL\gl3w.h"
#include "SDL\include\SDL.h"
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

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	//windowUI = SDL_CreateWindow("ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext glcontext = SDL_GL_CreateContext(App->window->GetWindow());
	gl3wInit();	

	show_test_window = true;
	show_another_window = false;

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ImVec4 clear_color = ImColor(114, 144, 154);

	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	{
		static float f = 0.0f;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		if (ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}

	ImGui::Render();
	SDL_GL_SwapWindow(App->window->GetWindow());
	
	return ret;
}

// Called before quitting
bool ModuleImGui::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();

	return true;
}