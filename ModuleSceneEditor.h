#ifndef __ModuleSceneEditor_H__
#define __ModuleSceneEditor_H__

#include "Module.h"
#include "Primitive.h"
#include "Timer.h"
#include "MathGeo/Geometry/AABB.h"
#include <list>

class GameObject;
class Mesh;
class Quadtree;

class ModuleSceneEditor : public Module
{
public:
	ModuleSceneEditor(Application* app, bool startEnabled = true);
	~ModuleSceneEditor();

	bool Init(Configuration data);
	bool Start();
	bool CleanUp(Configuration data);

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void Draw();

	void SelectGameObject(LineSegment& picking);

	//Show Editor
	void ShowEditor();

	//Add Primitive
	void AddCube(vec3 size, vec3 pos = vec3(0,0,0));
	void AddCube1(vec3 size, vec3 pos = vec3(0, 0, 0));
	void AddCube2(vec3 size, vec3 pos = vec3(0, 0, 0));
	void AddCylinder(float radius, float height, vec3 pos = vec3(0, 0, 0));
	void AddSphere(float radius, vec3 pos = vec3(0, 0, 0));
	void AddPlane(float x, float y, float z, float d, vec3 pos = vec3(0, 0, 0));
	void AddPlaneNoGrid(float x, float y, float z, float d, vec3 pos = vec3(0, 0, 0));
	void AddCapsule(float radius, float height, vec3 pos = vec3(0, 0, 0));

	GameObject* GetRoot();
	Quadtree* GetQuadtree();
	void SetSelected(GameObject* selected);

	void InsertToQuadtree(GameObject* toAdd);
	void RecalculateQuadtree();

	GameObject* CreateNewGameObject(const char* path);
	void CreateEmptyGameObject();

	void WantToLoadScene(const char* fileTitle);
	void SaveScene(const char* fileTitle)const;
	void LoadScene(const char* fileTitle);

public:
	bool recalcTree = false;
private:
	//For now ----
	std::list<Primitive*> sceneObjects;
	std::list<Mesh*> sceneMeshes;

	GameObject* root;
	GameObject* selected = nullptr;
	Quadtree* tree = nullptr;

	bool wantToSave = false;
	bool wantToLoad = false;
	std::string loadPath;

	Timer saveLoadTimer;
};

#endif