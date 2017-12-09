#include "ComponentBone.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "Application.h"
#include "imgui-1.51\imgui.h"

ComponentBone::ComponentBone(Bone bone, ComponentMesh* mesh): bone(bone), attachedMesh(mesh), Component(Component_Bone)
{
	name = mesh->GetGameObject()->GetName();
	name += " Bone";
	toCheck.Start();
}

ComponentBone::~ComponentBone()
{
}

void ComponentBone::Update()
{
	/*ComponentTransform* cTrans = (ComponentTransform*)myGO->FindComponent(Component_Transform);
	float3 pos;
	Quat rot;
	float3 sca;
	float4x4 trans = cTrans->GetGlobalTransform();
	trans.Decompose(pos, rot, sca);
	float3 originalPos;
	Quat originalRot;
	float3 originalSca;
	originalTrans.Decompose(originalPos, originalRot, originalSca);

	float3 posDiff = originalPos - pos;*/

	ComponentTransform* cTrans = (ComponentTransform*)myGO->FindComponent(Component_Transform);
	float4x4 trans = cTrans->GetGlobalTransform();
	ComponentTransform* meshTrans = (ComponentTransform*)attachedMesh->GetGameObject()->FindComponent(Component_Transform);
	trans = trans * meshTrans->GetLocalTransform().Inverted();
	trans = trans * bone.offsetMatrix;

	float3 pos;
	Quat rot;
	float3 sca;

	trans.Decompose(pos, rot, sca);
	Mesh* deformableMesh = attachedMesh->GetMeshDeformable();
	const float* originalVertices = attachedMesh->GetVertices();
	for (int i = 0; i < bone.weights.size(); i++)
	{
		deformableMesh->vertices[bone.weights[i].vertexID * 3] = originalVertices[bone.weights[i].vertexID * 3] + (pos.x * bone.weights[i].weight);
		deformableMesh->vertices[bone.weights[i].vertexID * 3 + 1] = originalVertices[bone.weights[i].vertexID * 3 + 1] + (-pos.y * bone.weights[i].weight);
		deformableMesh->vertices[bone.weights[i].vertexID * 3 + 2] = originalVertices[bone.weights[i].vertexID * 3 + 2] + (-pos.z * bone.weights[i].weight);
	}
	attachedMesh->UpdateDeformable();
	toCheck.Start();
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

void ComponentBone::SetOriginalTrans(float4x4 trans)
{
	originalTrans = trans;
}
