#ifndef __ModuleSceneEditor_H__
#define __ModuleSceneEditor_H__

#include "Module.h"
#include "Primitive.h"
#include <list>

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
	void SetToWireframe(bool wframe);

	void AddCube(vec3 size, vec3 pos = vec3(0,0,0));
	void AddCube1(vec3 size, vec3 pos = vec3(0, 0, 0));
	void AddCube2(vec3 size, vec3 pos = vec3(0, 0, 0));
	void AddCylinder(float radius, float height, vec3 pos = vec3(0, 0, 0));
	void AddSphere(float radius, vec3 pos = vec3(0, 0, 0));

private:
	//For now ----
	std::list<Primitive*> sceneObjects;
	int geometryID;
	//--------

	bool wframe;
};

#endif