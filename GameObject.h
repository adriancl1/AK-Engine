#pragma once
#include "Component.h"
#include "MathGeo\Math\float3.h"

#include <string>
#include <vector>

class GameObject
{
public:
	GameObject(GameObject* parent = nullptr);
	~GameObject();

	void Update();
	void AddChild(GameObject* child);
	void DeleteChilds();
	void AddComponent(Component* component);
	void SetName(const char* name);
	void OnEditor();

	Component* FindComponent(ComponentType type) const;

	std::vector<Component*> components;
	std::vector<GameObject*> childs;
protected:
	GameObject* parent;
	std::string name;
};
