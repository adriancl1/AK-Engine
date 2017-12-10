#include "ComponentParticles.h"
#include "Configuration.h"

ComponentParticles::ComponentParticles() : Component(Component_Mesh)
{

}

ComponentParticles::~ComponentParticles()
{

}

void ComponentParticles::Update()
{
}

void ComponentParticles::OnSave(Configuration & data) const
{
	data.SetFloat("duration", duration);

	data.SetBool("looping", looping);
	data.SetBool("prewarm", prewarm);

	data.SetFloat("startDelay", startDelay);
	data.SetFloat("startLifeTime", startLifeTime);
	data.SetFloat("startSpeed", startSpeed);

	data.SetBool("startSize3D", startSize3D);
	data.SetFloat("startSize", startSize);
	data.SetBool("startRotation3D", startRotation3D);
	data.SetFloat("startRotation", startRotation);

	data.SetFloat("randomizeRotation", randomizeRotation);

	//color is miss

	//Gravity modifier
	//enum simulation Space 
	data.SetBool("simulationSpeed", simulationSpeed);
	//enum scaling mode -local -world
	data.SetBool("playOnAwake", playOnAwake);
	data.SetInt("maxParticles", maxParticles);

	data.SetBool("playOnAwake", playOnAwake);
	data.SetInt("seed", seed);
}

void ComponentParticles::OnLoad(Configuration & data)
{

	duration = data.GetFloat("duration");
	looping = data.GetBool("looping");
	prewarm = data.GetBool("prewarm");

	startDelay = data.GetFloat("startDelay");
	startLifeTime = data.GetFloat("startLifeTime");
	startSpeed = data.GetFloat("startSpeed");


	startSize3D = data.GetBool("startSize3D");
	startSize = data.GetFloat("startSize");
	startRotation3D = data.GetBool("startRotation3D");
	startRotation = data.GetFloat("startRotation");

	randomizeRotation = data.GetFloat("randomizeRotation");
	//color is miss

	//Gravity modifier
	//enum simulation Space 
	simulationSpeed = data.GetFloat("simulationSpeed");
	//enum scaling mode -local -world
	playOnAwake = data.GetBool("playOnAwake");
	maxParticles = data.GetInt("maxParticles");

	autoRandomSeed = data.GetBool("autoRandomSeed");
	seed = data.GetInt("seed");

}
