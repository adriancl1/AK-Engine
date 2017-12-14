#include "ComponentRig.h"
#include "ResourceRig.h"
#include "Application.h"
#include "ComponentTransform.h"
#include "GameObject.h"

#include <gl/GL.h>
#include <gl/GLU.h>


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

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

void ComponentRig::Update()
{
	if (assignedBones == false)
	{
		rig->LoadToGameObject(myGO, &bonesGO);
		assignedBones = true;
	}
}

void ComponentRig::PostUpdate()
{
	if (debug)
	{
		DrawDebug();
	}
}

void ComponentRig::DrawDebug() const
{
	for (int i = 0; i < bonesGO.size(); i++)
	{
		GameObject* tmp = bonesGO[i];
		ComponentTransform* originTrans = (ComponentTransform*)tmp->FindComponent(Component_Transform);
		for (int j = 0; j < tmp->GetChilds().size(); j++)
		{
			ComponentTransform* destinTrans = (ComponentTransform*)tmp->GetChilds()[j]->FindComponent(Component_Transform);
			glColor3f(0, 1, 0);
			glBegin(GL_LINES);
			glVertex3f(originTrans->GetGlobalPosition().x, originTrans->GetGlobalPosition().y, originTrans->GetGlobalPosition().z);
			glVertex3f(destinTrans->GetGlobalPosition().x, destinTrans->GetGlobalPosition().y, destinTrans->GetGlobalPosition().z);
			glEnd();
		}	
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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
		ImGui::Checkbox("Debug", &debug);
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
}