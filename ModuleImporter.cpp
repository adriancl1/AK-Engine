#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleSceneEditor.h"
#include "ModuleResources.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ResourceMesh.h"
#include "Quadtree.h"
#include "MeshImporter.h"

#include "Glew/include/glew.h"
#include "MathGeo/Math/Quat.h"

#include <cstdio>

#include "Assimp/include/cimport.h" 
#include "Assimp/include/scene.h" 
#include "Assimp/include/postprocess.h" 
#include "Assimp/include/cfileio.h"

#include "mmgr/mmgr.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "importer";
}

ModuleImporter::~ModuleImporter()
{
	delete meshImporter;
}

bool ModuleImporter::Init(Configuration data)
{
	meshImporter = new MeshImporter();

	struct aiLogStream stream; 
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr); 
	aiAttachLogStream(&stream);
	return true;
}

GameObject* ModuleImporter::LoadGameObject(const char* fullPath)
{
	GameObject* newObject = new GameObject();

	const aiScene* scene = aiImportFile(fullPath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("Scene %s loaded succesfully", fullPath);

		//Load transform
		aiNode* node = scene->mRootNode;
		newObject->SetName(node->mName.C_Str());
		newObject->AddComponent(LoadTransform(node));

		LOG("Loading meshes");
		std::string tmpPath = fullPath;
		uint length = tmpPath.length();	

		uint i = tmpPath.find_last_of("\\");
		length = length - i - 1;
		char* fileName = new char[length + 1];
		tmpPath.copy(fileName, length, i + 1);
		fileName[length] = '\0';

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			scene->mMeshes[i]->mName = fileName;
			scene->mMeshes[i]->mName.Append(std::to_string(i).c_str());
		}

		delete[] fileName;

		LoadNodes(node, scene, newObject);

		aiReleaseImport(scene);

		return newObject;
	}
	else
	{
		LOG("Error loading scene %s", fullPath);
		return nullptr;
	}
}

void ModuleImporter::LoadNewTexture(const char* fullPath)
{
	/*int count = 0;
	for (int i = 0; i < App->sceneEditor->GetRoot()->GetChilds().size(); i++)
	{
		count = 0;
		for (int j = 0; j < App->sceneEditor->GetRoot()->GetChilds()[i]->GetComponents().size(); j++)
		{
			if (App->sceneEditor->GetRoot()->GetChilds()[i]->GetComponents()[j]->GetType() == Component_Material)
			{
				dynamic_cast<ComponentMaterial*>(App->sceneEditor->GetRoot()->GetChilds()[i]->GetComponents()[j])->OverrideTexture(fullPath);
				count++;
			}
		}

		//In case it didn't have any previous material
		if (count == 0)
		{
			ComponentMaterial* newMat = new ComponentMaterial();
			newMat->idTexture = App->textures->ImportImage(fullPath);
			newMat->SetName(fullPath);
			App->sceneEditor->GetRoot()->GetChilds()[i]->AddComponent(newMat);
		}
	}
	LOG("Set %s as new texture for current meshes.");*/
}

void ModuleImporter::LoadOwnFormat(const char * path, ResourceMesh* mesh) const
{
	meshImporter->Load(path, mesh);
}

bool ModuleImporter::SaveOwnFormat(aiMesh * mesh, const char * UID)
{
	return meshImporter->Save(mesh, UID);
}

void ModuleImporter::LoadNodes(aiNode* node, const aiScene* scene, GameObject* addTo)
{
	GameObject* newObject = new GameObject();
	addTo->AddChild(newObject);
	newObject->SetName(node->mName.C_Str());

	newObject->AddComponent(LoadTransform(node));

	//In case of nodes with multiple meshes ----
	std::vector<GameObject*> tmpVector;
	tmpVector.push_back(newObject);

	for (int i = 0; i < (int)(node->mNumMeshes - 1); i++)
	{
		GameObject* vecGO = new GameObject();
		addTo->AddChild(vecGO);
		vecGO->SetName(node->mName.C_Str());
		vecGO->AddComponent(LoadTransform(node));
		tmpVector.push_back(vecGO);
	}
	//--------
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* newMesh = scene->mMeshes[node->mMeshes[i]];
		if (newMesh != nullptr)
		{
			ComponentMesh* m = new ComponentMesh();

			int meshUID = App->resources->ImportFile(newMesh->mName.C_Str(), newMesh);
			if (meshUID != -1)
			{
				m->AddResource(meshUID);
			}

			aiMaterial* material = nullptr;
			material = scene->mMaterials[newMesh->mMaterialIndex];
			tmpVector[i]->AddComponent(LoadMaterial(material));

			m->SetName("Mesh");

			m->GetEnclosingBox().SetNegativeInfinity();

			m->GetEnclosingBox().Enclose((float3*)m->GetVertices(), m->GetNumVertices());
			
			tmpVector[i]->AddComponent(m);
		}
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		LoadNodes(node->mChildren[i], scene, newObject);
	}
}

ComponentMaterial* ModuleImporter::LoadMaterial(aiMaterial* newMaterial)
{
	ComponentMaterial* m = new ComponentMaterial;
	//MATERIAL

	if (newMaterial != nullptr)
	{
		uint numTextures = newMaterial->GetTextureCount(aiTextureType_DIFFUSE);
		aiString path;

		newMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		uint length = path.length;

		std::string tmpPath = path.C_Str();

		uint i = tmpPath.find_last_of("\\");
		length = length - i - 1;
		char* fileName = new char[length + 1];
		tmpPath.copy(fileName, length, i + 1);
		fileName[length] = '\0';

		std::string fullPath = "Assets/";
		fullPath.append(fileName);

		int texUID = App->resources->ImportFile(fullPath.c_str(), Resource_Texture);
		if (texUID != -1)
		{
			m->AddResource(texUID);
		}
		m->SetName(fileName);

		delete[] fileName;
		fileName = nullptr;

		return m;
	}
	return nullptr;
}

ComponentTransform* ModuleImporter::LoadTransform(aiNode* node)
{
	aiVector3D translation;
	aiVector3D scale;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scale, rotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 sca(scale.x, scale.y, scale.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	ComponentTransform* trans = new ComponentTransform(pos, sca, rot);

	return trans;
}


bool ModuleImporter::CleanUp(Configuration data)
{
	return true;
}