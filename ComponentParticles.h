#pragma once

#include "Component.h"

#include "MathGeo\MathGeoLib.h"
#include "ParticleSystem\ParticleSystem.h"

class ComponentParticles : public Component
{
public:
	ComponentParticles();
	~ComponentParticles();

	void Update(float dt) override;

	void OnPlay() override;
	void OnPause() override;

	void OnEditor()override;

	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;

private:
	ParticleSystem * particleSystem = nullptr;



	//this must work on API

private:// Usuary data
	//basic stats---------------------------------------------------
	float duration = 5.f;

	bool looping = true;
	bool prewarm = true;

	float startDelay = 0.f;
	float startLifeTime = 5.f;
	float startSpeed = 5.f;

	bool startSize3D = false;
	float startSize = 1.f;
	bool startRotation3D = false;
	float startRotation = 0;

	float randomizeRotation = 0.f; //may be between 0 & 1
	float3 color = { 0.f,0.f,0.f }; // RGB

	//Gravity modifier
	//enum simulation Space 
	float simulationSpeed = 1.f;
	//enum scaling mode -local -world

	bool playOnAwake = true;
	int maxParticles = 1000;

	bool autoRandomSeed = true;
	int seed = 0;
	//---------------------------------------------------------------
};