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
	PositionKey currentPosKey;
	PositionKey nextPosKey;
	RotationKey currentRotKey; 
	RotationKey nextRotKey;
	animTimer += (float)anim->ticksPerSecond / anim->duration;
	animTimer += 0.08f * speedFactor;
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
			currentPosKey = anim->bones[i].posKeys[0];
			nextPosKey = anim->bones[i].posKeys[0];
			currentRotKey = anim->bones[i].rotKeys[0];
			nextRotKey = anim->bones[i].rotKeys[0];
		}
		else if (animTimer > anim->duration)
		{
			animTimer = 0;
			currentPosKey = anim->bones[i].posKeys[0];
			nextPosKey = anim->bones[i].posKeys[0];
			currentRotKey = anim->bones[i].rotKeys[0];
			nextRotKey = anim->bones[i].rotKeys[0];
		}
		else
		{
			for (int j = 0; j < anim->bones[i].posKeys.size(); j++)
			{
				if (anim->bones[i].posKeys[j].time < animTimer)
				{
					currentPosKey = anim->bones[i].posKeys[j];
					if (anim->bones[i].posKeys.size() > j + 1)
					{
						nextPosKey = anim->bones[i].posKeys[j + 1];
					}
					else
					{
						nextPosKey = anim->bones[i].posKeys[j];
					}
					
				}
			}
			for (int j = 0; j < anim->bones[i].rotKeys.size(); j++)
			{
				if (anim->bones[i].rotKeys[j].time < animTimer)
				{
					currentRotKey = anim->bones[i].rotKeys[j];
					if (anim->bones[i].rotKeys.size() > j + 1)
					{
						nextRotKey= anim->bones[i].rotKeys[j + 1];
					}
					else
					{
						nextRotKey = anim->bones[i].rotKeys[j];
					}

				}
			}
		}
		float time;
		float3 bonePos;
		Quat boneRot;

		if (nextPosKey.time != currentPosKey.time)
		{
			time = (animTimer - currentPosKey.time) / (nextPosKey.time - currentPosKey.time);
			bonePos = float3::Lerp(currentPosKey.value, nextPosKey.value, time);
		}
		else
		{
			bonePos = currentPosKey.value;
		}
		if (nextRotKey.time != currentRotKey.time)
		{
			time = (animTimer - currentRotKey.time) / (nextRotKey.time - currentRotKey.time);
			boneRot = Quat::Slerp(currentRotKey.value, nextRotKey.value, time);
		}
		else
		{
			boneRot = currentRotKey.value;
		}
		
		boneTrans->SetPosition(bonePos);
		boneTrans->SetRotation(boneRot);
	}
}

void ComponentAnimation::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Duration : %f", anim->duration);
		ImGui::Text("Ticks Per Second : %i", anim->ticksPerSecond);
		ImGui::DragFloat("TIME TEST", &animTimer, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("Speed Factor", &speedFactor, 0.01f, 0.01f, 4.0f);
		if (ImGui::Button("Delete Component"))
		{
			wantsToDie = true;
		}
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
