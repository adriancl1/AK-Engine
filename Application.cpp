#include "Application.h"
#include "parson\parson.h"
#include "Brofiler-1.1.2\Brofiler.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	imGui = new ModuleImGui(this);
	sceneEditor = new ModuleSceneEditor(this);
	importer = new ModuleImporter(this);
	textures = new ModuleTextures(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(imGui);
	AddModule(importer);
	AddModule(sceneEditor);
	AddModule(textures);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;

	BROFILER_CATEGORY("Aplication Init", Profiler::Color::AliceBlue);

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	JSON_Value * configValue = json_parse_file("config.json");
	JSON_Object * configObject = json_value_get_object(configValue);

	while(item != NULL && ret == true)
	{
		ret = item->data->Init(json_object_dotget_object(configObject, item->data->name.c_str()));
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		BROFILER_CATEGORY("%s Init", item->data->name.c_str(), Brofiler::Color::AliceBlue);

		ret = item->data->Start();
		item = item->next;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	ms_timer.Start();
	startTime = (float)ms_timer.Read() / 1000.0f;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f - startTime;
	lastFPS = 1.0f / dt;
	lastMs = (float) ms_timer.Read();
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	JSON_Value* configValue = json_parse_file("config.json");
	JSON_Object* objectData = json_value_get_object(configValue);

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp(objectData);
		item = item->prev;
	}
	json_serialize_to_file(configValue, "config.json");

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

float Application::GetFPS()
{
	return lastFPS;
}

float Application::GetMs()
{
	return lastMs;
}