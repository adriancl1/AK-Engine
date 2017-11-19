#include "GameObject.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneEditor.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"

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
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->Update();
	}
}

void GameObject::PostUpdate()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end();)
	{
		if ((*it)->wantsToDie)
		{
			delete(*it);
			(*it) = nullptr;
			it = components.erase(it);
		}
		else
		{
			it++;
		}
	}
	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end();)
	{
		if ((*it)->wantsToDie)
		{
			App->sceneEditor->recalcTree = true;
			delete(*it);
			(*it) = nullptr;
			it = childs.erase(it);
		}
		else
		{
			(*it)->PostUpdate();
			it++;
		}
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

void GameObject::UpdateChildsTransform()
{
	for (int i = 0; i < childs.size(); i++)
	{
		ComponentTransform* tmp = (ComponentTransform*)childs[i]->FindComponent(Component_Transform);
		if (tmp != nullptr)
		{
			tmp->UpdateTrans();
			childs[i]->UpdateChildsTransform();
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
	ImGui::SetNextWindowPos(ImVec2(w - PROPERTIES_WIDTH - 5, 25));

	std::string temp = name;
	temp += " properties";
	ImGui::Begin(temp.c_str());
	
	ImGui::PushItemWidth(-140);
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->OnEditor();
	}
	if (components.size() < 4)//Transform, mesh and texture
	{
		if (addingMesh == false && FindComponent(Component_Mesh) == nullptr && ImGui::Button("Add Component Mesh"))
		{
			addingMesh = true;
		}
		if (addingMaterial == false &&FindComponent(Component_Material) == nullptr && ImGui::Button("Add Component Material"))
		{
			addingMaterial = true;
		}
	}

	if (addingMesh == true)
	{
		ImGui::Begin("Select new mesh");
		ImGui::PushItemWidth(-140);

		std::vector<Resource*> tmp = App->resources->GetResourcesOfType(Resource_Mesh);
		for (int i = 0; i < tmp.size(); i++)
		{
			if (ImGui::Button(tmp[i]->GetFile()))
			{
				ComponentMesh* newMesh = new ComponentMesh();
				newMesh->AddResource(tmp[i]->GetUID());
				AddComponent(newMesh);
				addingMesh = false;
			}
		}
		if (ImGui::Button("Cancel"))
		{
			addingMesh = false;
		}
		ImGui::End();
	}

	if (addingMaterial == true)
	{
		ImGui::Begin("Select new material texture");
		ImGui::PushItemWidth(-140);

		std::vector<Resource*> tmp = App->resources->GetResourcesOfType(Resource_Texture);
		for (int i = 0; i < tmp.size(); i++)
		{
			if (ImGui::Button(tmp[i]->GetFile()))
			{
				ComponentMaterial* newMat = new ComponentMaterial();
				newMat->AddResource(tmp[i]->GetUID());
				AddComponent(newMat);
				addingMaterial = false;
			}
		}
		if (ImGui::Button("Cancel"))
		{
			addingMaterial = false;
		}
		ImGui::End();
	}

	if (ImGui::Button("Delete Game object"))
	{
		App->sceneEditor->SetSelected(nullptr);
		wantsToDie = true;
	}

	ImGui::End();
}

void GameObject::RecursiveDraw()
{
	App->renderer3D->Draw(this);
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->RecursiveDraw();
	}
}

void GameObject::CollectAllIntersectionsAABB(std::vector<GameObject*>& intersections, LineSegment & line)
{
	ComponentMesh* cMesh = (ComponentMesh*)FindComponent(Component_Mesh);

	if (cMesh != nullptr)
	{
		if (cMesh->IntersectsAABB(line) == true)
		{
			intersections.push_back(this);
		}
	}

	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->CollectAllIntersectionsAABB(intersections, line);
	}
}

bool GameObject::Intersects(LineSegment & line) const
{
	ComponentMesh* cMesh = (ComponentMesh*)FindComponent(Component_Mesh);

	if (cMesh != nullptr)
	{
		return cMesh->IntersectsAABB(line);
	}
	else
	{
		return false;
	}
}

void GameObject::CollectTriIntersections(LineSegment & line, float & distance, float3 & hitPoint)
{
	ComponentMesh* cMesh = (ComponentMesh*)FindComponent(Component_Mesh);

	if (cMesh != nullptr)
	{
		cMesh->TriIntersection(line, distance, hitPoint);
	}
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

	int componentSize = dataToLoad.GetArraySize("Components");

	for (int i = 0; i < componentSize; i++)
	{
		Configuration componentConfig(dataToLoad.GetArray("Components", i));
		ComponentType cType = (ComponentType)componentConfig.GetInt("Type");
		switch (cType)
		{
		case Component_Camera:
		{
			ComponentCamera* compCamera = new ComponentCamera();
			compCamera->OnLoad(componentConfig);
			AddComponent(compCamera);
			break;
		}
		case Component_Mesh:
		{
			ComponentMesh* compMesh = new ComponentMesh();
			compMesh->OnLoad(componentConfig);
			AddComponent(compMesh);
			break;
		}
		case Component_Transform:
		{
			ComponentTransform* compTrans = new ComponentTransform();
			AddComponent(compTrans);
			compTrans->OnLoad(componentConfig);			
			break;
		}
		case Component_Material:
		{
			ComponentMaterial* compMat = new ComponentMaterial();
			compMat->OnLoad(componentConfig);
			AddComponent(compMat);
			break;
		}
		default:
		{
			LOG("Error in component %i of %s, unknown type", &i, name.c_str());
			break;
		}
		}
	}
}

void GameObject::InsertSelfAndChilds()
{
	if (isStatic)
	{
		App->sceneEditor->InsertToQuadtree(this);
	}
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->InsertSelfAndChilds();
	}
}

void GameObject::OnStaticChange()
{
	if (isStatic == true)
	{
		App->sceneEditor->recalcTree = true;
	}
	else
	{
		App->sceneEditor->recalcTree = true;
		for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->isStatic = false;
			childs[i]->OnStaticChange();
		}
	}
}

std::vector<GameObject*> GameObject::GetChilds() const
{
	return childs;
}

std::vector<Component*> GameObject::GetComponents() const
{
	return components;
}