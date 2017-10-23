#include "Component.h"
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