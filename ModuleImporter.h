#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Globals.h"
#include "Module.h"

class aiMesh;
class aiMaterial;
class aiNode;
class aiScene;

class GameObject;

class MeshImporter;

class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init(Configuration data);
	bool CleanUp(Configuration data);

	GameObject* LoadGameObject(const char* fullPath);

	void LoadNodes(aiNode* root, const aiScene* scene, GameObject* addTo);
	ComponentMaterial* LoadMaterial(aiMaterial* newMaterial);
	ComponentTransform* LoadTransform(aiNode* node);

	void LoadNewTexture(const char* fullPath);
	void LoadOwnFormat(const char* path, ComponentMesh* mesh) const;

private:
	MeshImporter* meshImporter = nullptr;
};

#endif //__ModuleImporter_H__
