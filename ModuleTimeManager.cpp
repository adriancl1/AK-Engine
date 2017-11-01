#include "Globals.h"
#include "Application.h"
#include "ModuleTimeManager.h"

#include <stdio.h>


ModuleTimeManager::ModuleTimeManager(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "time manager";

	frameCount = 0;

	timeScale = 0.0f;
	deltatime = 0.0f;
	rTimeDeltaTime = 0.0f;

	playOneFrame = false;
	isGamePaused = false;
}

ModuleTimeManager::~ModuleTimeManager()
{
}

bool ModuleTimeManager::Init(Configuration data)
{
	return true;
}

bool ModuleTimeManager::CleanUp(Configuration data)
{
	return true;
}

update_status ModuleTimeManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTimeManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleTimeManager::PauseGame(bool pause)
{
	isGamePaused = pause;

	if (pause == true)
	{
		time.Stop();
	}
	if (pause == false)
	{
		time.Continue();
	}
}

void ModuleTimeManager::PlayOneFrame()
{
	playOneFrame = true;
}


bool ModuleTimeManager::IsGamePaused()
{
	return isGamePaused;
}

void ModuleTimeManager::StartTime()
{
	time.Start();
}

void ModuleTimeManager::StartRealTime()
{
	rTimeSinceStartup.Start();
}

void ModuleTimeManager::StopTime()
{
	time.Stop();
}

void ModuleTimeManager::StopRealTime()
{
	rTimeSinceStartup.Stop();
}

//Set
void ModuleTimeManager::SetFrameCount(uint amount)
{
	frameCount += amount;
}

//Get
uint ModuleTimeManager::GetFrameCount() const
{
	return frameCount;
}

float ModuleTimeManager::GetTime()
{
	return (float)time.Read()/1000.0f;
}

float ModuleTimeManager::GetTimeScale() const
{
	return timeScale;
}

float ModuleTimeManager::GetDeltaTime() const
{
	return deltatime;
}

float ModuleTimeManager::GetRealTime()
{
	return (float)rTimeSinceStartup.Read() / 1000.0f;
}

float ModuleTimeManager::GetRealDeltaTime() const
{
	return rTimeDeltaTime;
}

void ModuleTimeManager::OnConfiguration()
{
	if (ImGui::CollapsingHeader("Time Status"))
	{
		if (ImGui::Button("Play"))
		{
			PauseGame(false);
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			PauseGame(true);
		}
		ImGui::SameLine();
		if (ImGui::Button("Play One Frame"))
		{
			PlayOneFrame();
		}

		ImGui::Text("Total Frames: %i", GetFrameCount());
		ImGui::Text("Game Time Clock: %.2f", GetTime());
		ImGui::Text("Real Time Clock: %.2f", GetRealTime());
	}
}