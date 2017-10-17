#include "GameObject.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMesh.h"

GameObject::GameObject(GameObject* parent): parent(parent)
{
	name = "Game Object";
}
GameObject::~GameObject()
{
	while (!childs.empty())
	{
		delete childs.back();
		childs.pop_back();
	}
	childs.clear();
	while (!components.empty())
	{
		delete components.back();
		components.pop_back();
	}
	components.clear();
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

void GameObject::DeleteChilds()
{
	while (!childs.empty())
	{
		delete childs.back();
		childs.pop_back();
	}
	childs.clear();
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
	component->SetGameObject(this);
}

void GameObject::SetName(const char * name)
{
	this->name.assign(name);
}

Component* GameObject::FindComponent(ComponentType type) const
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type)
		{
			return components[i];
		}
	}

	return nullptr;
}

void GameObject::OnEditor()
{
	if (strcmp(name.c_str(), "Root") == 0)
	{
		for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->OnEditor();
		}
	}
	else
	{
		if (ImGui::TreeNodeEx(name.c_str()))
		{
			for (int i = 0; i < components.size(); i++)
			{
				components[i]->OnEditor();
			}
			for (int i = 0; i < childs.size(); i++)
			{
				childs[i]->OnEditor();
			}
			ImGui::TreePop();
		}
	}
}

void GameObject::Move(float3 newPos)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == Component_Mesh)
		{
			dynamic_cast<ComponentMesh*>(components[i])->Move(newPos);
		}
	}
}
