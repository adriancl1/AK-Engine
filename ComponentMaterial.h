#pragma once

#include "Component.h"
#include "Globals.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial() : Component(Component_Material)
	{}

	~ComponentMaterial()
	{}

	uint idTexture;
	
	void OnEditor() override
	{
		if (ImGui::TreeNodeEx("Material"))
		{
			ImGui::Text(name.c_str());
			ImGui::Text("Texture ID: %i", idTexture);
			ImGui::TreePop();
		}
	}

	void OverrideTexture(const char* path)
	{
		idTexture = App->textures->ImportImage(path);
	}
};