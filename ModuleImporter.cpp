#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleSceneEditor.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

#include "Assimp\include\cimport.h" 
#include "Assimp\include\scene.h" 
#include "Assimp\include\postprocess.h" 
#include "Assimp\include\cfileio.h"
#include "Glew\include\glew.h"
#include "MathGeo\Math\Quat.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "importer";
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init(JSON_Object* data)
{
	struct aiLogStream stream; 
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr); 
	aiAttachLogStream(&stream);
	return true;
}

GameObject* ModuleImporter::LoadGameObject(const char* fullPath)
{
	GameObject* newObject = new GameObject();

	uint length = strlen(fullPath);

	std::string namePath = fullPath;

	uint i = namePath.find_last_of("\\");
	char* testM = new char[length-i];
	namePath.copy(testM, length - i, i);
	newObject->SetName(testM);

	delete[] testM;
	testM = nullptr;

	const aiScene* scene = aiImportFile(fullPath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("Scene %s loaded succesfully", fullPath);

		//Load transform
		aiNode* node = scene->mRootNode;
		newObject->AddComponent(LoadTransform(node));

		LOG("Loading meshes");

		LoadMesh(node, scene, newObject);

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
	int count = 0;
	for (int i = 0; i < App->sceneEditor->GetRoot()->childs.size(); i++)
	{
		count = 0;
		for (int j = 0; j < App->sceneEditor->GetRoot()->childs[i]->components.size(); j++)
		{
			if (App->sceneEditor->GetRoot()->childs[i]->components[j]->GetType() == Component_Material)
			{
				dynamic_cast<ComponentMaterial*>(App->sceneEditor->GetRoot()->childs[i]->components[j])->OverrideTexture(fullPath);
				count++;
			}
		}

		//In case it didn't have any previous material
		if (count == 0)
		{
			ComponentMaterial* newMat = new ComponentMaterial();
			newMat->idTexture = App->textures->ImportImage(fullPath);
			newMat->SetName(fullPath);
			App->sceneEditor->GetRoot()->childs[i]->AddComponent(newMat);
		}
	}
	LOG("Set %s as new texture for current meshes.");
}

ComponentMesh* ModuleImporter::LoadMesh(aiNode* node, const aiScene* scene, GameObject* addTo)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		ComponentMesh* m = new ComponentMesh;
		aiMesh* newMesh = scene->mMeshes[node->mMeshes[i]];

		if (newMesh != nullptr)
		{
			//VERTICES
			m->numVertices = newMesh->mNumVertices;

			m->vertices = new float[m->numVertices * 3];

			memcpy(m->vertices, newMesh->mVertices, sizeof(float)* m->numVertices * 3);

			LOG("New mesh with %d vertices", m->numVertices);

			glGenBuffers(1, (GLuint*)&m->idVertices);
			glBindBuffer(GL_ARRAY_BUFFER, m->idVertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->numVertices * 3, m->vertices, GL_STATIC_DRAW);

			//INDICES
			if (newMesh->HasFaces())
			{
				m->numIndices = newMesh->mNumFaces * 3;
				m->indices = new uint[m->numIndices];

				for (uint i = 0; i < newMesh->mNumFaces; ++i)
				{
					if (newMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m->indices[i * 3], newMesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}

				glGenBuffers(1, (GLuint*)&m->idIndices);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->idIndices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m->numIndices, m->indices, GL_STATIC_DRAW);
			}

			//NORMALS
			if (newMesh->HasNormals())
			{
				m->normals = new float[m->numVertices * 3];
				memcpy(m->normals, newMesh->mNormals, sizeof(float) * m->numVertices * 3);

				glGenBuffers(1, (GLuint*) &(m->idNormals));
				glBindBuffer(GL_ARRAY_BUFFER, m->idNormals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->numVertices * 3, m->normals, GL_STATIC_DRAW);
			}

			//COLORS
			if (newMesh->HasVertexColors(0))
			{
				m->colors = new float[m->numVertices * 3];
				memcpy(m->colors, newMesh->mColors, sizeof(float) * m->numVertices * 3);

				glGenBuffers(1, (GLuint*) &(m->idColors));
				glBindBuffer(GL_ARRAY_BUFFER, m->idColors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->numVertices * 3, m->colors, GL_STATIC_DRAW);
			}

			//TEXTURE COORDS
			if (newMesh->HasTextureCoords(0))
			{
				m->texCoords = new float[m->numVertices * 3];
				memcpy(m->texCoords, newMesh->mTextureCoords[0], sizeof(float) * m->numVertices * 3);

				glGenBuffers(1, (GLuint*) &(m->idTexCoords));
				glBindBuffer(GL_ARRAY_BUFFER, m->idTexCoords);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->numVertices * 3, m->texCoords, GL_STATIC_DRAW);

				aiMaterial* material = nullptr;
				material = scene->mMaterials[newMesh->mMaterialIndex];
				addTo->AddComponent(LoadMaterial(material));

			}

			m->SetName(node->mName.C_Str());

			m->enclosingBox.SetNegativeInfinity();

			m->enclosingBox.Enclose((float3*)m->vertices, m->numVertices);

			addTo->AddComponent(m);
		}
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		LoadMesh(node->mChildren[i], scene, addTo);
	}

	return nullptr;
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

		std::string fullPath = "Assets/";
		fullPath.append(path.C_Str());
		m->idTexture = App->textures->ImportImage(fullPath.c_str());
		m->SetName(path.C_Str());

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

	return new ComponentTransform(pos, sca, rot);
}


bool ModuleImporter::CleanUp(JSON_Object* data)
{
	return true;
}

