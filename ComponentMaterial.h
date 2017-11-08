#pragma once

#include "Component.h"

class ResourceTexture;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();
	
	void OnEditor() override;

	void OverrideTexture(const char* path);

	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;

	void AddResource(int UID) override;

	int GetTextureID() const;

	ResourceTexture* tex = nullptr;
};