#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImGui.h"
#include "ModuleSceneEditor.h"
#include "ModuleGeometryImporter.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleImGui* imGui;
	ModuleSceneEditor* sceneEditor;
	ModuleGeometryImporter* geometryImporter;


private:

	Timer	ms_timer;
	float	dt;
	float startTime;
	float lastFPS = 0;
	float lastMs = 0;
	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	float GetFPS();
	float GetMs();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

// Give App pointer access everywhere
extern Application* App;