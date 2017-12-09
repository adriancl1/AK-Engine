#include "ComponentAnimation.h"
#include "ResourceAnimation.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"

ComponentAnimation::ComponentAnimation() : Component(Component_Animation)
{
	name = "Animation";
}

ComponentAnimation::~ComponentAnimation()
{
}

void ComponentAnimation::Update()
{
	//animTimer += (float)anim->ticksPerSecond / anim->duration;
	bool foundBone = false;
	for (int i = 0; i < anim->bones.size(); i++)
	{
		GameObject* boneGO = myGO->FindByName(anim->bones[i].name.c_str());
		ComponentTransform* boneTrans;
		if (boneGO != nullptr)
		{
			boneTrans = (ComponentTransform*)boneGO->FindComponent(Component_Transform);
		}
		else
		{
			LOG("Couldn't finde bone with name %s! Deleting animation component.", anim->bones[i].name.c_str())
				wantsToDie = true;
			return;
		}

		if (animTimer == 0)
		{
			boneTrans->SetPosition(anim->bones[i].posKeys[0].value);
			foundBone == true;
		}
		else
		{
			for (int j = 0; j < anim->bones[i].posKeys.size(); j++)
			{
				if (anim->bones[i].posKeys[j].time > animTimer)
				{
					boneTrans->SetPosition(anim->bones[i].posKeys[j].value);
					foundBone == true;
					break;
				}
			}
		}
	}
	if (foundBone == false)
	{
		//animTimer = 0;
	}
}

void ComponentAnimation::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Duration : %f", anim->duration);
		ImGui::Text("Ticks Per Second : %i", anim->ticksPerSecond);
		ImGui::DragFloat("TIME TEST", &animTimer, 0.01f, 0.0f, 10.0f);
		ImGui::TreePop();
	}
}

void ComponentAnimation::OnSave(Configuration & data) const
{
}

void ComponentAnimation::OnLoad(Configuration & data)
{
}

void ComponentAnimation::AddResource(int UID)
{
	anim = (ResourceAnimation*)App->resources->Get(UID);
	if (anim != nullptr)
	{
		anim->LoadToComponent();
	}
}

void ComponentAnimation::SetBonePos(float3 value) const
{
}
