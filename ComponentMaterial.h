#pragma once

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	uint idTexture;
	
	void OnEditor() override;

	void OverrideTexture(const char* path);

	std::string texName;

	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;
};