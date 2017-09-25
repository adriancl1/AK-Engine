#include "Globals.h"
#include "Application.h"
#include "Brofiler-1.1.2\Brofiler.h"
#include "ModuleWindow.h"
#include "parson\parson.h"


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
	App->imGui->AddLogToWindow("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		App->imGui->AddLogToWindow("SDL_VIDEO could not initialize!");
		ret = false;
	}
	else
	{

		if (data == nullptr)
		{
			App->imGui->AddLogToWindow("Window config couldn't load, using default values!");
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
			App->imGui->AddLogToWindow("Window config loaded");

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
			App->imGui->AddLogToWindow("Window could not be created!");
			ret = false;
		}
		else
		{
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
	App->imGui->AddLogToWindow("Destroying SDL window and quitting all SDL systems"); 
	
	/* TODO: It should save the configuration */
	json_object_dotset_number(data, "width", width);
	json_object_dotset_number(data, "height", height);
	json_object_dotset_number(data, "brightness", brightness);
	json_object_dotset_boolean(data, "fullscreen", fullscreen);
	json_object_dotset_boolean(data, "fullDesktop", fullDesktop);
	json_object_dotset_boolean(data, "borderless", borderless);

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
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

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	Uint32 flags;
	if (fullscreen == true)
	{
		this->fullscreen = true;
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	else if (fullscreen == false)
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

void ModuleWindow::SetBorderless(bool borderless)
{
	Uint32 flags;
	if (borderless == true)
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

void ModuleWindow::SetFullDesktop(bool fullDesktop)
{
	Uint32 flags;
	if (fullDesktop == true)
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