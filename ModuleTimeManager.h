#pragma once

#include "Module.h"
#include "Timer.h"
#include "Globals.h"


class ModuleTimeManager : public Module
{
public:
	ModuleTimeManager(Application* app, bool start_enabled = true);
	~ModuleTimeManager();

	bool Init(Configuration data);
	bool CleanUp(Configuration data);

	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

	void PauseGame(bool pause);
	void PlayOneFrame();
	bool IsGamePaused();

	void StartTime();
	void StartRealTime();
	void StopTime();
	void StopRealTime();

	void SetFrameCount(uint amount);

	uint GetFrameCount() const;
	float GetTime(); //Time since Startup  //Game Clock
	float GetTimeScale() const; //Scale at which time is passing //Game Clock
	float GetDeltaTime() const; //Last frame time expressed in seconds //Game Clock
	float GetRealTime(); //Real Time since Startup //Real Time Clock
	float GetRealDeltaTime() const; //Last frame time expressed in seconds //Real Time Clock

	void OnConfiguration() override;
private:
	uint frameCount;

	Timer time;
	float timeScale;
	float deltatime;
	Timer rTimeSinceStartup;
	float rTimeDeltaTime;

	bool playOneFrame;
	bool isGamePaused;
};

