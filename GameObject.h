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
	void SetLocalTransform();
	void UpdateChildsTransform();
	void OnEditor();
	void ShowProperties();

	Component* FindComponent(ComponentType type) const;
	GameObject* GetParent() const;

	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	bool selected = false;
protected:
	GameObject* parent;
	std::string name;
};
