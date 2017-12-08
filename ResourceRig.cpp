#include "ResourceRig.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"

ResourceRig::ResourceRig(int UID) : Resource(UID, Resource_Mesh)
{
}

ResourceRig::~ResourceRig()
{
}

bool ResourceRig::LoadToGameObject(GameObject* GO)
{
	ComponentTransform* goTrans = (ComponentTransform*)GO->FindComponent(Component_Transform);
	if (goTrans != nullptr)
	{
		float3 goPos;
		Quat goRot;
		float3 goScale;
		goTrans->GetGlobalTransform().Decompose(goPos, goRot, goScale);

		for (uint i = 0; i < bones.size(); i++)
		{
			float3 bonePos;
			Quat boneRot;
			float3 boneScale;
			bones[i].offsetMatrix.Decompose(bonePos, boneRot, boneScale);

			GameObject* tmp = new GameObject();
			ComponentTransform* tmpTrans = new ComponentTransform(goPos + bonePos, goScale, goRot * boneRot);
			tmp->AddComponent(tmpTrans);
			tmp->SetName(bones[i].name.c_str());
			GO->AddChild(tmp);
		}
	}
	else
	{
		for (uint i = 0; i < bones.size(); i++)
		{
			float3 bonePos;
			Quat boneRot;
			float3 boneScale;
			bones[i].offsetMatrix.Decompose(bonePos, boneRot, boneScale);

			GameObject* tmp = new GameObject();
			ComponentTransform* tmpTrans = new ComponentTransform(bonePos, boneScale, boneRot);
			GO->AddChild(tmp);
		}
	}
	return true;
}

bool ResourceRig::LoadInMemory()
{
	App->importer->LoadRigOwnFormat(exportedFile.c_str(), this);
	return true;
	return false;
}

bool ResourceRig::UnloadFromMemory()
{
	return false;
}

int ResourceRig::GetBoneNum() const
{
	return bones.size();
}
