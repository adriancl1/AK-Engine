#include "GameObject.h"
#include "Application.h"
#include "ModuleRenderer3D.h"


GameObject::GameObject(GameObject* parent): parent(parent)
{

}
GameObject::~GameObject()
{

}

void GameObject::Update()
{
	App->renderer3D->Draw(this);

	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->Update();
	}

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
}

void GameObject::AddChild(GameObject* child)
{
	childs.push_back(child);
	child->parent = this;
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

Component* GameObject::FindComponent(ComponentType type)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type)
		{
			return components[i];
		}
	}

	return nullptr;
}

void GameObject::OnEditor()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->OnEditor();
	}
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->OnEditor();
	}
}