#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#include "Brofiler-1.1.2\Brofiler.h"

#include "mmgr\mmgr.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);

	name = "input";
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init(JSON_Object* data)
{
	BROFILER_CATEGORY("Module Input Init", Profiler::Color::AliceBlue);

	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Module Input PreUpdate", Profiler::Color::AliceBlue);

	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	char* fileDir = nullptr;
	SDL_Event e;
	uint length;

	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
				if (e.wheel.y == 1)
				{
					App->camera->ZoomIn();
				}
				if (e.wheel.y == -1)
				{
					App->camera->ZoomOut();
				}
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_DROPFILE:      // In case if dropped file
				fileDir = e.drop.file;
				// Shows directory of dropped file
				LOG("%s dropped on window.", fileDir);
				length = strlen(fileDir);
				if (strcmp(&fileDir[length - 4], ".fbx") == 0 || strcmp(&fileDir[length - 4], ".FBX") == 0)
				{
					App->sceneEditor->CreateNewGameObject(fileDir);
				}
				else if (strcmp(&fileDir[length - 4], ".wav") == 0 || strcmp(&fileDir[length - 4], ".WAV") == 0)
				{
					App->audio->PlayMusic(fileDir);
				}
				else if (strcmp(&fileDir[length - 4], ".png") == 0 || strcmp(&fileDir[length - 4], ".PNG") == 0)
				{
					App->importer->LoadNewTexture(fileDir);		
				}
				else
				{
					LOG("Unknown file format!");
				}
				SDL_free(fileDir);    // Free dropped_filedir memory
				break;

			case SDL_WINDOWEVENT:
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
				break;
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp(JSON_Object* data)
{
	LOG("Quitting SDL input event subsystem.");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::OnConfiguration()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse X: %i | Mouse Y: %i", GetMouseX(), GetMouseY());
	}
}
