#pragma once

#include <string>
#include <vector>
#include "Component.h"

class GameObject
{
public:
	GameObject(GameObject* parent = nullptr);
	~GameObject();

	void Update();
	void AddChild(GameObject* child);
	void AddComponent(Component* component);

	Component* FindComponent(ComponentType type);

	GameObject* parent;

	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;
};
