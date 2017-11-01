#include "GameObject.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#define PROPERTIES_WIDTH 300
#define PROPERTIES_HEIGHT 500

GameObject::GameObject(GameObject* parent, bool isStatic): parent(parent), isStatic(isStatic)
{
	name = "Game Object";
	uid = App->randomGenerator->Int();
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
		childs.back() = nullptr;
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

const char * GameObject::GetName() const
{
	return name.c_str();
}

int GameObject::GetUID() const
{
	return uid;
}

void GameObject::SetLocalTransform()
{
	ComponentTransform* myTrans = (ComponentTransform*)FindComponent(Component_Transform);
	if (myTrans != nullptr)
	{
		myTrans->SetLocalTrans(GetParent());
	}
}

void GameObject::UpdateChildsTransform()
{
	for (int i = 0; i < childs.size(); i++)
	{
		ComponentTransform* tmp = (ComponentTransform*)childs[i]->FindComponent(Component_Transform);
		if (tmp != nullptr)
		{
			tmp->UpdateTransFromParent(this);
		}
	}
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

GameObject * GameObject::GetParent() const
{
	return parent;
}

GameObject* GameObject::FindByUID(int toFind)
{
	if (toFind == uid)
	{
		return this;
	}

	GameObject* ret = nullptr;
	for (int i = 0; i < childs.size() && ret == nullptr; i++)
	{
		ret = childs[i]->FindByUID(toFind);
	}

	return ret;

}

void GameObject::OnEditor()
{
	if (strcmp(name.c_str(), "Root") != 0)
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
	else
	{
		for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->OnEditor();
		}
	}
}

void GameObject::ShowProperties()
{
	int w, h;
	App->window->GetWindowSize(w, h);
	ImGui::SetNextWindowSize(ImVec2(PROPERTIES_WIDTH, PROPERTIES_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(w - PROPERTIES_WIDTH, 0));

	std::string temp = name;
	temp += " properties";
	ImGui::Begin(temp.c_str());
	
	ImGui::PushItemWidth(-140);
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->OnEditor();
	}
	ImGui::End();
}

void GameObject::OnSerialize(Configuration& dataToSave) const
{
	if (strcmp(name.c_str(), "Root") != 0)
	{
		Configuration myConf;

		myConf.SetString("Name", name.c_str());
		myConf.SetInt("UID", uid);
		myConf.SetInt("Parent UID", parent != App->sceneEditor->GetRoot() ? parent->GetUID() : 0);

		myConf.AddArray("Components");

		for (int i = 0; i < components.size(); i++)
		{
			Configuration compConfig;
			compConfig.SetInt("Type", components[i]->GetType());
			components[i]->OnSave(compConfig);
			myConf.AddArrayEntry(compConfig);
		}
		dataToSave.AddArrayEntry(myConf);
	}

	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->OnSerialize(dataToSave);
	}
}


void GameObject::OnDeserialize(Configuration& dataToLoad)
{
	SetName(dataToLoad.GetString("Name"));
	uid = dataToLoad.GetInt("UID");
	int parentUID = dataToLoad.GetInt("Parent UID");
	if (parentUID == 0)
	{
		App->sceneEditor->GetRoot()->AddChild(this);
	}
	else
	{
		GameObject* parent = App->sceneEditor->GetRoot()->FindByUID(parentUID);
		if (parent != nullptr)
		{
			parent->AddChild(this);
		}
	}
}