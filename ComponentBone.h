#pragma once

#include "Component.h"
#include "ResourceRig.h"

struct Bone;

class ComponentBone : public Component
{
public:
	ComponentBone(Bone bone);
	~ComponentBone();

	void Update();

	void OnEditor() override;

	void DrawDebug() const;

private:
	Bone bone;
};