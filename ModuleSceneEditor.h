#ifndef __ModuleSceneEditor_H__
#define __ModuleSceneEditor_H__

#include "Module.h"
#include "Primitive.h"
#include "Timer.h"
#include "MathGeo/Geometry/AABB.h"
#include "MathGeo/MathGeoLib.h"//To test
#include <list>

class GameObject;
class Mesh;
class Quadtree;

class Emiter;	//To test

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

	GameObject* GetRoot();
	Quadtree* GetQuadtree();
	GameObject* GetSelected()const;
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
	GameObject* root;
	GameObject* selected = nullptr;
	Quadtree* tree = nullptr;

	bool wantToSave = false;
	bool wantToLoad = false;
	std::string loadPath;


	Timer saveLoadTimer;


	//To test
	Sphere spheretest;
	Emiter* emiterTest = nullptr;
	//
};

#endif