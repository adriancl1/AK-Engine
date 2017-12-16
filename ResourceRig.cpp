#include "ResourceRig.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentBone.h"
#include "ComponentMesh.h"

ResourceRig::ResourceRig(int UID) : Resource(UID, Resource_Rig)
{
}

ResourceRig::~ResourceRig()
{
}

bool ResourceRig::LoadToGameObject(GameObject* GO, std::vector<GameObject*>* toFill)
{
	for (int i = 0; i < bones.size(); i++)
	{
		GameObject* tmp = GO->GetParent()->FindByName(bones[i].name.c_str());
		if (tmp != nullptr)
		{
			ComponentMesh* tmpMesh = (ComponentMesh*)GO->FindComponent(Component_Mesh);
			if (tmpMesh->GetMeshDeformable() == nullptr)
			{
				tmpMesh->createDeformable();
			}
			ComponentBone* tmpBone = new ComponentBone(bones[i], tmpMesh);
			tmp->AddComponent(tmpBone);
			ComponentTransform* trans = (ComponentTransform*)GO->FindComponent(Component_Transform);
			trans->SetScale(float3(0.03f, 0.03f, 0.03f));
			toFill->push_back(tmp);
		}
	}
	return true;
}

bool ResourceRig::LoadInMemory()
{
	App->importer->LoadRigOwnFormat(exportedFile.c_str(), this);
	return true;
}

bool ResourceRig::UnloadFromMemory()
{
	return false;
}

int ResourceRig::GetBoneNum() const
{
	return bones.size();
}
