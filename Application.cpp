#include "Application.h"
#include "parson\parson.h"
#include "Brofiler-1.1.2\Brofiler.h"

#define MAX_FPS_MS_COUNT 81

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
	hardware = new ModuleHardware(this);

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
	AddModule(hardware);

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

void Application::OnConfiguration()
{
	CycleFPSAndMsData(GetFPS(), GetMs());

	if (ImGui::CollapsingHeader("Application"))
	{
		char frameMStitle[25];
		sprintf_s(frameMStitle, 25, "Framerate %.1f", FPSData[FPSData.size() - 1]);
		ImGui::PlotHistogram("##framerate", &FPSData[0], FPSData.size(), 0, frameMStitle, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(frameMStitle, 25, "Milliseconds %0.1f", MsData[MsData.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &MsData[0], MsData.size(), 0, frameMStitle, 0.0f, 40.0f, ImVec2(310, 100));
	}

	p2List_item<Module*>* item = list_modules.getLast();
	item = list_modules.getFirst();

	while (item != NULL)
	{
		item->data->OnConfiguration();
		item = item->next;
	}
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

void Application::CycleFPSAndMsData(float fps, float ms)
{
	//FPS
	if (FPSData.size() >= MAX_FPS_MS_COUNT)
	{
		for (int i = 0; i < MAX_FPS_MS_COUNT - 2; i++)
		{
			FPSData[i] = FPSData[i + 1];
		}
		FPSData[MAX_FPS_MS_COUNT - 1] = fps;
	}
	else
	{
		FPSData.push_back(fps);
	}

	//MS
	if (MsData.size() >= MAX_FPS_MS_COUNT)
	{
		for (int i = 0; i < MAX_FPS_MS_COUNT - 2; i++)
		{
			MsData[i] = MsData[i + 1];
		}
		MsData[MAX_FPS_MS_COUNT - 1] = ms;
	}
	else
	{
		MsData.push_back(ms);
	}
}