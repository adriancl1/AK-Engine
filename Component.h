#pragma once

#include <string>

enum ComponentType
{
	Component_Transform,
	Component_Mesh,
	Component_Material
};

class Component
{
public:
	Component(ComponentType type, bool startActive = true) : type(type), active(startActive)
	{}

	virtual void Enable()
	{
		active = true;
	}
	virtual void Update()
	{
		
	}
	virtual void Disable()
	{
		active = false;
	}

	std::string name;
	bool active;
	ComponentType type;
};
