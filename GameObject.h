#pragma once

#include <string>
#include <vector>
#include "Component.h"

class GameObject
{
public:

	void Update();
	Component* FindComponent(ComponentType type);

	std::string name;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> childs;
};
