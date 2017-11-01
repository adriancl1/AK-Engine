#include "Component.h"
#include "Configuration.h"
#include "GameObject.h"

Component::Component(ComponentType type, bool startActive) : type(type), active(startActive)
{}

Component::~Component()
{}

void Component::Enable()
{
	active = true;
}

void Component::Update()
{}

void Component::Disable()
{
	active = false;
}

void Component::OnEditor()
{}

void Component::OnSave(Configuration& data) const
{
}

void Component::OnLoad(Configuration& data)
{
}

bool Component::IsActive() const
{
	return active;
}

void Component::SetName(const char* name)
{
	this->name.assign(name);
}

const char* Component::GetName() const
{
	return name.c_str();
}

void Component::SetGameObject(GameObject * GO)
{
	myGO = GO;
}

GameObject * Component::GetGameObject() const
{
	return myGO;
}

ComponentType Component::GetType() const
{
	return type;
}