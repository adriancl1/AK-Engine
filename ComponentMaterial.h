#pragma once

#include "Component.h"
#include "Globals.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial() : Component(Component_Material)
	{}

	uint idTexture;
	
	void OnEditor() override
	{
		ImGui::Text("Name: %s", name);
		ImGui::Text("Texture ID: %i", idTexture);
	}
};