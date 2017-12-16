#pragma once

#include "Globals.h"
#include "Component.h"

#include "MathGeo/Math/float3.h"
#include <list>

enum AnimationStatus
{
	ANIMATION_PLAY,
	ANIMATION_PAUSE,
	ANIMATION_STOP,
	ANIMATION_BLENDING
};

struct Animation
{
	Animation(const char* name) : name(name){};
	std::string name;
	float startTime;
	float endTime;
};

class ResourceAnimation;

class ComponentAnimation : public Component
{
public:
	ComponentAnimation();
	~ComponentAnimation();

	void Update()override;

	void OnEditor()override;

	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;
	void AddResource(int UID) override;

	void SetBonePos(float3 value)const;

private:
	ResourceAnimation* anim;
	float animTimer = 0;
	float lastAnimTimer = 0;
	float blendDuration = 1.0f;
	float blendTimer = 0;
	float speedFactor = 1;
	bool blending = true;

	AnimationStatus animStatus;
	Animation* currentAnimation = nullptr;
	Animation* lastAnimation = nullptr;
	std::list<Animation*> animationList;
};