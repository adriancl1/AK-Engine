#include "ComponentParticles.h"
#include "Configuration.h"

ComponentParticles::ComponentParticles() : Component(Component_Mesh)
{

}

ComponentParticles::~ComponentParticles()
{
	if (particle != nullptr)
	{
		particle->UnloadFromComponent();
	}
}

void ComponentParticles::Update()
{
}

void ComponentParticles::OnSave(Configuration & data) const
{


}

void ComponentParticles::OnLoad(Configuration & data)
{
	AddResource(data.GetInt("Resource UID"));
}

void ComponentParticles::AddResource(int UID)
{
	
}
