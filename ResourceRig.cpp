#include "ResourceRig.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentBone.h"

ResourceRig::ResourceRig(int UID) : Resource(UID, Resource_Mesh)
{
}

ResourceRig::~ResourceRig()
{
}

bool ResourceRig::LoadToGameObject(GameObject* GO)
{
	for (int i = 0; i < bones.size(); i++)
	{
		GameObject* tmp = GO->GetParent()->FindByName(bones[i].name.c_str());
		if (tmp != nullptr)
		{
			ComponentBone* tmpBone = new ComponentBone(bones[i], (ComponentMesh*)GO->FindComponent(Component_Mesh));
			tmp->AddComponent(tmpBone);

			ComponentTransform* tmpTrans = (ComponentTransform*)tmp->FindComponent(Component_Transform);
			if (tmpTrans != nullptr)
			{
				tmpTrans->UpdateTrans();
				tmpBone->SetOriginalTrans(tmpTrans->GetGlobalTransform());
			}
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
