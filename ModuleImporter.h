#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Globals.h"
#include "Module.h"

class aiMesh;
class aiMaterial;
class aiNode;
class aiScene;

class GameObject;

class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init(JSON_Object* data = nullptr);
	bool CleanUp(JSON_Object* data = nullptr);

	GameObject* LoadGameObject(const char* fullPath);

	void LoadNodes(aiNode* root, const aiScene* scene, GameObject* addTo);
	ComponentMaterial* LoadMaterial(aiMaterial* newMaterial);
	ComponentTransform* LoadTransform(aiNode* node);

	void LoadNewTexture(const char* fullPath);

	void Load(ComponentMesh* mesh, const char* inputFile);
	void Save(ComponentMesh* mesh, const char* outputFile);
};

#endif //__ModuleImporter_H__
