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
	std::list<Animation*>::iterator item = animationList.end();
	item--;
	while (item != animationList.begin())
	{
		RELEASE((*item));
		item--;
	}
	if (item == animationList.begin())
	{
		RELEASE((*item));
	}

	animationList.clear();
}

void ComponentAnimation::Update()
{
	PositionKey currentPosKey;
	PositionKey nextPosKey;
	RotationKey currentRotKey;
	RotationKey nextRotKey;

	bool foundBone = false;

	switch (AnimStatus)
	{
	case ANIMATION_PLAY:
		animTimer += ((float)anim->ticksPerSecond / anim->duration) * speedFactor;

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

			if (animTimer == currentAnimation->startTime)
			{
				currentPosKey = anim->bones[i].posKeys[0];
				nextPosKey = anim->bones[i].posKeys[0];
				currentRotKey = anim->bones[i].rotKeys[0];
				nextRotKey = anim->bones[i].rotKeys[0];
			}
			else if (animTimer > currentAnimation->endTime)
			{
				animTimer = currentAnimation->startTime;
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
							nextRotKey = anim->bones[i].rotKeys[j + 1];
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
		break;

		case ANIMATION_PAUSE:
			break;

		case ANIMATION_STOP:
			animTimer = currentAnimation->startTime;				
			break;
		case ANIMATION_BLENDING:
		{
			PositionKey lasAnimCurrentPosKey;
			PositionKey lastAnimNextPosKey;
			RotationKey lastAnimCurrentRotKey;
			RotationKey lastAnimNextRotKey;
			animTimer += ((float)anim->ticksPerSecond / anim->duration) * speedFactor;
			lastAnimTimer += ((float)anim->ticksPerSecond / anim->duration) * speedFactor;
			blendTimer += ((float)anim->ticksPerSecond / anim->duration) * speedFactor;

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

				//Current new Animation -----
				if (animTimer == currentAnimation->startTime)
				{
					currentPosKey = anim->bones[i].posKeys[0];
					nextPosKey = anim->bones[i].posKeys[0];
					currentRotKey = anim->bones[i].rotKeys[0];
					nextRotKey = anim->bones[i].rotKeys[0];
				}
				else if (animTimer > currentAnimation->endTime)
				{
					animTimer = currentAnimation->startTime;
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
								nextRotKey = anim->bones[i].rotKeys[j + 1];
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
				// ------
				//Last Anim -----
				if (lastAnimTimer == lastAnimation->startTime)
				{
					lasAnimCurrentPosKey = anim->bones[i].posKeys[0];
					lastAnimNextPosKey = anim->bones[i].posKeys[0];
					lastAnimCurrentRotKey = anim->bones[i].rotKeys[0];
					lastAnimNextRotKey = anim->bones[i].rotKeys[0];
				}
				else if (lastAnimTimer > lastAnimation->endTime)
				{
					lastAnimTimer = lastAnimation->startTime;
					lasAnimCurrentPosKey = anim->bones[i].posKeys[0];
					lastAnimNextPosKey = anim->bones[i].posKeys[0];
					lastAnimCurrentRotKey = anim->bones[i].rotKeys[0];
					lastAnimNextRotKey = anim->bones[i].rotKeys[0];
				}
				else
				{
					for (int j = 0; j < anim->bones[i].posKeys.size(); j++)
					{
						if (anim->bones[i].posKeys[j].time < lastAnimTimer)
						{
							lasAnimCurrentPosKey = anim->bones[i].posKeys[j];
							if (anim->bones[i].posKeys.size() > j + 1)
							{
								lastAnimNextPosKey = anim->bones[i].posKeys[j + 1];
							}
							else
							{
								lastAnimNextPosKey = anim->bones[i].posKeys[j];
							}

						}
					}
					for (int j = 0; j < anim->bones[i].rotKeys.size(); j++)
					{
						if (anim->bones[i].rotKeys[j].time < lastAnimTimer)
						{
							lastAnimCurrentRotKey = anim->bones[i].rotKeys[j];
							if (anim->bones[i].rotKeys.size() > j + 1)
							{
								lastAnimNextRotKey = anim->bones[i].rotKeys[j + 1];
							}
							else
							{
								lastAnimNextRotKey = anim->bones[i].rotKeys[j];
							}

						}
					}
				}
				float lastAnimTime;
				float3 lastAnimBonePos;
				Quat lastAnimBoneRot;

				if (lastAnimNextPosKey.time != lasAnimCurrentPosKey.time)
				{
					lastAnimTime = (lastAnimTimer - lasAnimCurrentPosKey.time) / (lastAnimNextPosKey.time - lasAnimCurrentPosKey.time);
					lastAnimBonePos = float3::Lerp(lasAnimCurrentPosKey.value, lastAnimNextPosKey.value, lastAnimTime);
				}
				else
				{
					lastAnimBonePos = lasAnimCurrentPosKey.value;
				}
				if (lastAnimNextRotKey.time != lastAnimCurrentRotKey.time)
				{
					lastAnimTime = (lastAnimTimer - lastAnimCurrentRotKey.time) / (lastAnimNextRotKey.time - lastAnimCurrentRotKey.time);
					lastAnimBoneRot = Quat::Slerp(lastAnimCurrentRotKey.value, lastAnimNextRotKey.value, lastAnimTime);
				}
				else
				{
					lastAnimBoneRot = lastAnimCurrentRotKey.value;
				}
				// ------
				float3 blendedPos = float3::Lerp(lastAnimBonePos, bonePos, (blendTimer / blendDuration));
				Quat blendedRot = Quat::Slerp(lastAnimBoneRot, boneRot, (blendTimer / blendDuration));
				boneTrans->SetPosition(blendedPos);
				boneTrans->SetRotation(blendedRot);
			}
			if (blendTimer > blendDuration)
			{
				blendTimer = 0;
				AnimStatus = ANIMATION_PLAY;
			}
			break;
		}
		default:
			break;
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
		ImGui::Text("Playing %s animation", currentAnimation->name.c_str());
		ImGui::DragFloat("Start time", &currentAnimation->startTime, 0.2f, 0.0f, currentAnimation->endTime - 0.1f);
		ImGui::DragFloat("End time", &currentAnimation->endTime, 0.2f, currentAnimation->startTime + 0.1f, anim->duration);

		if (ImGui::Button("Play"))
		{
			AnimStatus = ANIMATION_PLAY;
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			AnimStatus = ANIMATION_PAUSE;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			for (int i = 0; i < anim->bones.size(); i++)
			{
				GameObject* boneGO = myGO->FindByName(anim->bones[i].name.c_str());
				ComponentTransform* boneTrans;
				if (boneGO != nullptr)
				{
					boneTrans = (ComponentTransform*)boneGO->FindComponent(Component_Transform);
				}
				boneTrans->SetPosition(anim->bones[i].posKeys[0].value);
				boneTrans->SetRotation(anim->bones[i].rotKeys[0].value);

			}
			AnimStatus = ANIMATION_STOP;
		}

		if (ImGui::Button("Add Animation"))
		{
			std::string tmpName = "Animation ";
			tmpName += std::to_string(animationList.size());
			Animation* newAnim = new Animation(tmpName.c_str());
			newAnim->startTime = 0.0f;
			newAnim->endTime = anim->duration;
			animationList.push_back(newAnim);
		}

		for (std::list<Animation*>::iterator it = animationList.begin(); it != animationList.end(); it++)
		{
			ImGui::Text("Animation: %s", (*it)->name.c_str());
			ImGui::SameLine();
			std::string tmpSelect = "Select##";
			tmpSelect += (*it)->name;
			if (ImGui::Button(tmpSelect.c_str()))
			{
				if (currentAnimation != (*it))
				{
					lastAnimTimer = animTimer;
					lastAnimation = currentAnimation;
					currentAnimation = (*it);
					animTimer = (*it)->startTime;
					if (blending)
					{
						AnimStatus = ANIMATION_BLENDING;
					}
					else
					{
						AnimStatus = ANIMATION_PLAY;
					}
				}
			}
		}

		ImGui::Checkbox("Blending", &blending);

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
		Animation* defaultAnim = new Animation("Default");
		defaultAnim->startTime = 0.0f;
		defaultAnim->endTime = anim->duration;
		currentAnimation = defaultAnim;
		animationList.push_back(defaultAnim);
	}
}

void ComponentAnimation::SetBonePos(float3 value) const
{
}
