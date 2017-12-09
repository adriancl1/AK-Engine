#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Globals.h"
#include "Module.h"

class aiMesh;
class aiMaterial;
class aiNode;
class aiScene;
class aiAnimation;

class GameObject;

class MeshImporter;
class RigImporter;
class AnimationImporter;

class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;

class ResourceMesh;
class ResourceRig;
class ResourceAnimation;

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
	void LoadRigOwnFormat(const char* path, ResourceRig* rig) const;
	void LoadAnimOwnFormat(const char* path, ResourceAnimation* animation) const;
	bool SaveMeshOwnFormat(aiMesh* mesh, const char* UID);
	bool SaveRigOwnFormat(aiMesh* mesh, const char* rigName);
	bool SaveAnimOwnFormat(aiAnimation* anim, const char* animName);

private:
	MeshImporter* meshImporter = nullptr;
	RigImporter* rigImporter = nullptr;
	AnimationImporter* animImporter = nullptr;
};

#endif //__ModuleImporter_H__
