#include "ComponentRig.h"
#include "ResourceRig.h"
#include "Application.h"

ComponentRig::ComponentRig() : Component(Component_Rig)
{
	name = "Rig";
}

ComponentRig::~ComponentRig()
{
	if (rig != nullptr)
	{
		rig->UnloadFromComponent();
	}
}

void ComponentRig::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		if (rig != nullptr)
		{
			ImGui::Text("Rig bones: %i", rig->GetBoneNum());
			ImGui::Text("Resource ID: %i", rig->GetUID());
			ImGui::Text("Resource reference counting: %i", rig->GetReferenceCount());
		}
		else
		{
			ImGui::Text("Missing resource rig!");
		}
		if (ImGui::Button("Delete Component"))
		{
			wantsToDie = true;
		}
		ImGui::TreePop();
	}
}

void ComponentRig::OnSave(Configuration & data) const
{
}

void ComponentRig::OnLoad(Configuration & data)
{
}

void ComponentRig::AddResource(int UID)
{
	rig = (ResourceRig*)App->resources->Get(UID);
	rig->LoadToComponent();
	rig->LoadToGameObject(myGO);
}