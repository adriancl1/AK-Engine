#include "Application.h"
#include "parson\parson.h"
#include "Brofiler-1.1.2\Brofiler.h"

#include "mmgr\mmgr.h"

#define MAX_FPS_MS_COUNT 81

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
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
	std::list<Module*>::iterator item = listModules.end();
	item--;
	while (item != listModules.begin())
	{
		RELEASE(item._Ptr->_Myval);
		item--;
	}
	if (item == listModules.begin())
	{
		RELEASE(item._Ptr->_Myval);
	}

	listModules.clear();
}

bool Application::Init()
{
	bool ret = true;
	debug = false;

	BROFILER_CATEGORY("Aplication Init", Profiler::Color::AliceBlue);

	// Call Init() in all modules
	std::list<Module*>::iterator item = listModules.begin();

	JSON_Value * configValue = json_parse_file("config.json");
	JSON_Object * configObject = json_value_get_object(configValue);

	while(item != listModules.end() && ret == true)
	{
		ret = item._Ptr->_Myval->Init(json_object_dotget_object(configObject, item._Ptr->_Myval->name.c_str()));
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = listModules.begin();

	while(item != listModules.end() && ret == true)
	{
		BROFILER_CATEGORY("%s Init", item._Ptr->_Myval->name.c_str(), Brofiler::Color::AliceBlue);

		ret = item._Ptr->_Myval->Start();
		item++;
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
	
	std::list<Module*>::iterator item = listModules.begin();
	
	while(item != listModules.end() && ret == UPDATE_CONTINUE)
	{
		ret = item._Ptr->_Myval->PreUpdate(dt);
		item++;
	}

	item = listModules.begin();

	while(item != listModules.end() && ret == UPDATE_CONTINUE)
	{
		ret = item._Ptr->_Myval->Update(dt);
		item++;
	}

	item = listModules.begin();

	while(item != listModules.end() && ret == UPDATE_CONTINUE)
	{
		ret = item._Ptr->_Myval->PostUpdate(dt);
		item++;
	}

	FinishUpdate();

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::iterator item = listModules.end();

	item--;

	JSON_Value* configValue = json_parse_file("config.json");
	JSON_Object* objectData = json_value_get_object(configValue);

	while (item != listModules.begin() && ret == true)
	{
		ret = item._Ptr->_Myval->CleanUp(objectData);
		item--;
	}
	if (item == listModules.begin() && ret == true)
	{
		ret = item._Ptr->_Myval->CleanUp(objectData);
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

	std::list<Module*>::iterator item = listModules.begin();

	while (item != listModules.end())
	{
		item._Ptr->_Myval->OnConfiguration();
		item++;
	}
}

void Application::AddModule(Module* mod)
{
	listModules.push_back(mod);
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

bool Application::GetDebug() const
{
	return debug;
}