#pragma once

#include "Component.h"
#include "Globals.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial() : Component(Component_Material)
	{
	}

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
};