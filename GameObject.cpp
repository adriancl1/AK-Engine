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
	ImGuiTreeNodeFlags flags = 0;
	if (childs.empty())
	{
		flags |= ImGuiTreeNodeFlags_Bullet;
	}
	if (selected == true)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
		if (ImGui::TreeNodeEx(this, flags, this->name.c_str()))
		{
			if (ImGui::IsItemHoveredRect() && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				App->sceneEditor->SetSelected(this);
			}
			for (int i = 0; i < childs.size(); i++)
			{
				childs[i]->OnEditor();
			}
			ImGui::TreePop();
		}
}

void GameObject::ShowProperties()
{
	ImGui::Begin("Properties");
	ImGui::PushItemWidth(-140);
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->OnEditor();
	}
	ImGui::End();
}
