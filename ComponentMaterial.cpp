#include "Application.h"
#include "ComponentMaterial.h"
#include "ResourceTexture.h"
#include "ModuleResources.h"
#include "imgui-1.51\imgui.h"

ComponentMaterial::ComponentMaterial() : Component(Component_Material)
{
}

ComponentMaterial::~ComponentMaterial()
{
	if (tex != nullptr)
	{
		tex->UnloadFromComponent();
	}
}

void ComponentMaterial::OnEditor()
{
	if (ImGui::TreeNodeEx("Material"))
	{
		ImGui::Text(name.c_str());
		if (tex != nullptr)
		{
			ImGui::Text("Texture ID: %i", tex->textureID);
			ImGui::Text("Resource ID: %i", tex->GetUID());
			ImGui::Text("Resource reference counting: %i", tex->GetReferenceCount());
			ImGui::Image((ImTextureID)tex->textureID, ImVec2(124, 124));
		}
		else
		{
			ImGui::Text("Missing resource texture!");
		}
		if (ImGui::Button("Delete Component"))
		{
			wantsToDie = true;
		}
		ImGui::TreePop();
	}
}

void ComponentMaterial::OverrideTexture(const char* path)
{
	//idTexture = App->textures->ImportImage(path);
}

void ComponentMaterial::OnSave(Configuration & data) const
{
	if (tex != nullptr)
	{
		data.SetInt("Texture UID", tex->GetUID());
	}
}

void ComponentMaterial::OnLoad(Configuration & data)
{
	AddResource(data.GetInt("Texture UID"));
}

void ComponentMaterial::AddResource(int UID)
{
	tex = (ResourceTexture*)App->resources->Get(UID);
	if (tex != nullptr)
	{
		tex->LoadToComponent();
	}	
}

int ComponentMaterial::GetTextureID() const
{
	if (tex != nullptr)
	{
		return tex->textureID;
	}
	else
	{
		return 0;
	}
}
