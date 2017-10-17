#include "Application.h"
#include "ComponentMaterial.h"
#include "imgui-1.51\imgui.h"

ComponentMaterial::ComponentMaterial() : Component(Component_Material)
{
}

ComponentMaterial::~ComponentMaterial()
{}

void ComponentMaterial::OnEditor()
{
	if (ImGui::TreeNodeEx("Material"))
	{
		ImGui::Text(name.c_str());
		ImGui::Text("Texture ID: %i", idTexture);
		ImGui::TreePop();
	}
}

void ComponentMaterial::OverrideTexture(const char* path)
{
	idTexture = App->textures->ImportImage(path);
}