#include "ComponentBone.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "Application.h"
#include "imgui-1.51\imgui.h"

#include <gl/GL.h>
#include <gl/GLU.h>


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

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
	ComponentTransform* cTrans = (ComponentTransform*)myGO->FindComponent(Component_Transform);
	float4x4 trans = cTrans->GetGlobalTransform();
	ComponentTransform* meshTrans = (ComponentTransform*)attachedMesh->GetGameObject()->FindComponent(Component_Transform);
	trans = meshTrans->GetLocalTransform().Inverted() * trans ;
	trans = trans * bone.offsetMatrix;

	float3 pos;
	Quat rot;
	float3 sca;

	//trans.Decompose(pos, rot, sca);
	Mesh* deformableMesh = attachedMesh->GetMeshDeformable();
	const float* originalVertices = attachedMesh->GetVertices();
	for (int i = 0; i < bone.weights.size(); i++)
	{
		int ind = bone.weights[i].vertexID;
		float3 originalVertex(&originalVertices[ind * 3]);
		float3 vertex(&deformableMesh->vertices[ind * 3]);

		vertex = trans.TransformPos(originalVertex);

		deformableMesh->vertices[ind * 3] += vertex.x * bone.weights[i].weight;
		deformableMesh->vertices[ind * 3 + 1] += vertex.y * bone.weights[i].weight;
		deformableMesh->vertices[ind * 3 + 2] += vertex.z * bone.weights[i].weight;
	}
	attachedMesh->UpdateDeformable();
	if (debug)
	{
		DrawDebug();
	}
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
		ImGui::Checkbox("Debug", &debug);
		ImGui::TreePop();
	}
}

void ComponentBone::DrawDebug() const
{
	Mesh* deformableMesh = attachedMesh->GetMeshDeformable();
	ComponentTransform* tmpTrans = (ComponentTransform*)attachedMesh->GetGameObject()->FindComponent(Component_Transform);

	glPushMatrix();
	if (tmpTrans != nullptr)
	{
		glMultMatrixf(tmpTrans->GetTransMatrix().Transposed().ptr());
	}

	for (int i = 0; i < bone.weights.size(); i++)
	{
		glColor3f(bone.weights[i].weight, 1.0f - bone.weights[i].weight, 0.0f);
		int ind = bone.weights[i].vertexID;
		float3 vertex(&deformableMesh->vertices[ind * 3]);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glVertex3f(vertex.x, vertex.y, vertex.z);
		glEnd();
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPopMatrix();
}
