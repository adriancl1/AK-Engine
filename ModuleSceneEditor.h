#ifndef __ModuleSceneEditor_H__
#define __ModuleSceneEditor_H__

#include "Module.h"
#include "Primitive.h"
#include "MathGeo\Geometry\AABB.h"
#include <list>

class GameObject;
class Mesh;

class ModuleSceneEditor : public Module
{
public:
	ModuleSceneEditor(Application* app, bool startEnabled = true);
	~ModuleSceneEditor();

	bool Init(JSON_Object* data = nullptr);
	bool Start();
	bool CleanUp(JSON_Object* data = nullptr);

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void Draw();

	//Show Editor
	void ShowEditor();

	GameObject* GetRoot();

	GameObject* CreateNewGameObject(const char* path);

private:
	GameObject* root;
};

#endif