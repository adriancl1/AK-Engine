#pragma once
#include "Component.h"
#include "MathGeo\Math\float3.h"

#include <string>
#include <vector>

class Configuration;

class GameObject
{
public:
	GameObject(GameObject* parent = nullptr, bool isStatic = false);
	~GameObject();

	void Update();
	void AddChild(GameObject* child);
	void DeleteChilds();
	void AddComponent(Component* component);
	void SetName(const char* name);

	const char* GetName() const;
	int GetUID()const;

	void SetLocalTransform();
	void UpdateChildsTransform();
	void OnEditor();
	void ShowProperties();

	void CollectIntersectionsAABB(std::vector<GameObject*>& intersections, LineSegment& line);
	void CollectTriIntersections(LineSegment& line, float& distance, float3& hitPoint);

	void OnSerialize(Configuration& dataToSave)const;
	void OnDeserialize(Configuration& dataToLoad);

	Component* FindComponent(ComponentType type) const;
	GameObject* GetParent() const;
	GameObject* FindByUID(int toFind);

	std::vector<GameObject*> GetChilds() const;
	std::vector<Component*> GetComponents() const;

	bool selected = false;
	bool isStatic;

protected:
	int uid;

	GameObject* parent;

	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;
};