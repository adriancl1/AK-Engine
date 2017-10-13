#include "Globals.h"
#include "Application.h"
#include "Brofiler-1.1.2\Brofiler.h"
#include "ModuleWindow.h"
#include "parson\parson.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	name = "window";
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(JSON_Object* data)
{
	BROFILER_CATEGORY("Module Window Init", Profiler::Color::AliceBlue);

	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		if (data == nullptr)
		{
			LOG("Wndow config couldn't load, using default values!");
			//Create window
			width = SCREEN_WIDTH * SCREEN_SIZE;
			height = SCREEN_HEIGHT * SCREEN_SIZE;
			brightness = 1;
			Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

			//Use OpenGL 2.1
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

			if (WIN_FULLSCREEN == true)
			{
				fullscreen = true;
				flags |= SDL_WINDOW_FULLSCREEN;
			}

			else
			{
				fullscreen = false;
			}

			if (WIN_RESIZABLE == true)
			{
				flags |= SDL_WINDOW_RESIZABLE;
			}

			if (WIN_BORDERLESS == true)
			{
				borderless = true;
				flags |= SDL_WINDOW_BORDERLESS;
			}

			else
			{
				borderless = false;
			}

			if (WIN_FULLSCREEN_DESKTOP == true)
			{
				fullDesktop = true;
				flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			}

			else
			{
				fullDesktop = false;
			}

			window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		}

		else
		{
			LOG("Window config loaded succesfully.")

			width = json_object_dotget_number(data, "width");
			height= json_object_dotget_number(data, "height");
			brightness = json_object_dotget_number(data, "brightness");
			fullscreen = json_object_dotget_boolean(data, "fullscreen");
			fullDesktop = json_object_dotget_boolean(data, "fullDesktop");
			borderless = json_object_dotget_boolean(data, "borderless");
			
			width = width * SCREEN_SIZE;
			height = height * SCREEN_SIZE;

			Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

			//Use OpenGL 2.1
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

			if (fullscreen == true)
			{
				flags |= SDL_WINDOW_FULLSCREEN;
			}

			if (WIN_RESIZABLE == true)
			{
				flags |= SDL_WINDOW_RESIZABLE;
			}

			if (borderless == true)
			{
				flags |= SDL_WINDOW_BORDERLESS;
			}

			if (fullDesktop == true)
			{
				flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			}

			window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

			SDL_SetWindowBrightness(window, brightness);
			
		}

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			SetTitle(TITLE);
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

SDL_Window * ModuleWindow::GetWindow() const
{
	return window;
}

// Called before quitting
bool ModuleWindow::CleanUp(JSON_Object* data)
{
	LOG("Destroying SDL window and quitting all SDL systems");
	
	JSON_Object* windowData = json_object_dotget_object(data, name.c_str());

	json_object_dotset_number(windowData, "width", width);
	json_object_dotset_number(windowData, "height", height);
	json_object_dotset_boolean(windowData, "fullscreen", fullscreen);
	json_object_dotset_boolean(windowData, "fullDesktop", fullDesktop);
	json_object_dotset_boolean(windowData, "borderless", borderless);
	json_object_dotset_number(windowData, "brightness", brightness);

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::OnConfiguration()
{
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Text("%s", title);

		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f, "%.2f"))
		{
			SetBrightness(brightness);
		}
		if (ImGui::SliderInt("Width", &width, 1, 1920))
		{
			ResizeWindow(width, height);
		}
		if (ImGui::SliderInt("Height", &height, 1, 1080))
		{
			ResizeWindow(width, height);
		}

		ImGui::Text("Refresh Rate:");
		ImGui::SameLine();
		SDL_DisplayMode dm;
		SDL_GetDesktopDisplayMode(0, &dm);
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		{
			LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		}
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "%d", dm.refresh_rate);

		if (ImGui::Checkbox("Fullscreen", &fullscreen))
		{
			SetFullscreen(fullscreen);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Windowed Borderless", &borderless))
		{
			SetBorderless(borderless);
		}
		if (ImGui::Checkbox("Full Desktop", &fullDesktop))
		{
			SetFullDesktop(fullDesktop);
		}
	}
}

const char* ModuleWindow::GetTitle() const
{
	return title;
}


void ModuleWindow::SetTitle(char* title)
{
	this->title = title;
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::ResizeWindow(int width, int height)
{
	this->width = width;
	this->height = height;
	SDL_SetWindowSize(window, width, height);
}

void ModuleWindow::GetWindowSize(int& width, int& height)
{
	width = this->width;
	height = this->height;
}

void ModuleWindow::SetFullscreen(bool fscreen)
{
	Uint32 flags;
	if (fscreen == true)
	{
		this->fullscreen = true;
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	else if (fscreen == false)
	{
		this->fullscreen = false;
	}
	SDL_SetWindowFullscreen(window, flags);
}

bool ModuleWindow::GetFullscreen()
{
	return fullscreen;
}

void ModuleWindow::SetBrightness(float value)
{
	brightness = value;
	SDL_SetWindowBrightness(window, value);
}

float ModuleWindow::GetBrightness()
{
	return brightness;
}

void ModuleWindow::SetBorderless(bool bdless)
{
	Uint32 flags;
	if (bdless == true)
	{
		borderless = true;
		SDL_SetWindowBordered(window, SDL_FALSE);
	}
	else
	{
		borderless = false;
		SDL_SetWindowBordered(window, SDL_TRUE);
	}
}

bool ModuleWindow::GetBorderless()
{
	return borderless;
}

void ModuleWindow::SetFullDesktop(bool fDesktop)
{
	Uint32 flags;
	if (fDesktop == true)
	{
		this->fullDesktop = true;
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	else
	{
		this->fullDesktop = false;
	}
	SDL_SetWindowFullscreen(window, flags);
}

bool ModuleWindow::GetFullDesktop()
{
	return fullDesktop;
}