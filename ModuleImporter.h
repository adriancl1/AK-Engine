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

class ResourceMesh;

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init(Configuration data);
	bool CleanUp(Configuration data);

	GameObject* LoadGameObject(const char* fullPath);

	ComponentMaterial* LoadMaterial(aiMaterial* newMaterial);
	ComponentTransform* LoadTransform(aiNode* node);

	void LoadNodes(aiNode* root, const aiScene* scene, GameObject* addTo);
	void LoadNewTexture(const char* fullPath);
	void LoadOwnFormat(const char* path, ResourceMesh* mesh) const;
	bool SaveOwnFormat(aiMesh* mesh, const char* UID);

private:
	MeshImporter* meshImporter = nullptr;
};

#endif //__ModuleImporter_H__
