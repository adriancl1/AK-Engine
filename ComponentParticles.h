#pragma once

#include "Component.h"

class ResourceParticles;

class ComponentParticles: public Component
{
public:
	ComponentParticles();
	~ComponentParticles();

	void Update() override;

	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;
	void AddResource(int UID) override;

private:
	ResourceParticles* particle = nullptr;
};
