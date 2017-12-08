#include "ComponentBone.h"
#include "imgui-1.51\imgui.h"

ComponentBone::ComponentBone(Bone bone): bone(bone), Component(Component_Bone)
{
	name = "Bone";
}

ComponentBone::~ComponentBone()
{
}

void ComponentBone::Update()
{
	
}

void ComponentBone::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Name: %s", bone.name.c_str());

		for (int i = 0; i < bone.weights.size(); i++)
		{
			ImGui::Text("Vertex %i: ID: %i, Weight: %.2f", i, bone.weights[i].vertexID, bone.weights[i].weight);
		}
		ImGui::TreePop();
	}
}

void ComponentBone::DrawDebug() const
{

}