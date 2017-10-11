#include "Application.h"
#include "GameObject.h"
#include "ModuleSceneEditor.h"
#include "Glew\include\glew.h"
#include "MathGeo\Geometry\Triangle.h"
#include "MathGeo\Math\float4x4.h"
#include "Brofiler-1.1.2\Brofiler.h"



ModuleSceneEditor::ModuleSceneEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	name = "Scene editor";
}
ModuleSceneEditor::~ModuleSceneEditor()
{
	while (!sceneObjects.empty())
	{
		delete sceneObjects.front();
		sceneObjects.pop_front();
	}

	while (!sceneMeshes.empty())
	{
		delete sceneMeshes.front();
		sceneMeshes.pop_front();
	}
}

bool ModuleSceneEditor::Init(JSON_Object* data)
{
	BROFILER_CATEGORY("Module SceneEditor Start", Profiler::Color::AliceBlue);

	return true;
}

bool ModuleSceneEditor::Start()
{
	//App->audio->PlayMusic("audio/walkwithme.ogg");
	App->camera->Move(vec3(0, 1, 0));

	root = new GameObject();
	root->name = "Root";

	return true;
}
bool ModuleSceneEditor::CleanUp(JSON_Object* data)
{
	delete root;
	return true;
}

update_status ModuleSceneEditor::PreUpdate(float dt)
{
	App->textures->ImportImage("Assets/Lenna.png");
	return UPDATE_CONTINUE;
}
update_status ModuleSceneEditor::Update(float dt)
{
	return UPDATE_CONTINUE;
}
update_status ModuleSceneEditor::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleSceneEditor::Draw()
{
	for (std::list<Primitive*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		(*it)->Render();
	}
	
	root->Update();

	pPlane p(0, 0, 0, 100);
	p.color = White;
	p.Render();
}

void ModuleSceneEditor::SetToWireframe(bool wframe)
{
	this->wframe = wframe;

	if (wframe == true)
	{
		for (std::list<Primitive*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			(*it)->wire = true;
		}
	}
	else
	{
		for (std::list<Primitive*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			(*it)->wire = false;
		}
	}
}

bool ModuleSceneEditor::GetWireframe()const
{
	return wframe;
}

void ModuleSceneEditor::ShowEditor()
{
		root->OnEditor();
	
}

void ModuleSceneEditor::AddCube(vec3 size, vec3 pos)
{
	Cube* cube = new Cube;
	cube->size.Set(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);

	if (wframe == true)
	{
		cube->wire = true;
	}

	sceneObjects.push_back(cube);

	App->physics->AddBody(*cube);
}

void ModuleSceneEditor::AddCube1(vec3 size, vec3 pos)
{
	Cube1* cube = new Cube1(size.x, size.y, size.z);
	cube->size.Set(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);

	if (wframe == true)
	{
		cube->wire = true;
	}

	sceneObjects.push_back(cube);

	App->physics->AddBody(*cube);
}

void ModuleSceneEditor::AddCube2(vec3 size, vec3 pos)
{
	Cube2* cube = new Cube2(size.x, size.y, size.z);
	cube->size.Set(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);

	if (wframe == true)
	{
		cube->wire = true;
	}

	sceneObjects.push_back(cube);

	App->physics->AddBody(*cube);
}

void ModuleSceneEditor::AddCylinder(float radius, float height, vec3 pos)
{
	pCylinder* cyl = new pCylinder;
	cyl->radius = radius;
	cyl->height = height;
	cyl->SetPos(pos.x, pos.y, pos.z);

	if (wframe == true)
	{
		cyl->wire = true;
	}

	sceneObjects.push_back(cyl);

	App->physics->AddBody(*cyl);
}

void ModuleSceneEditor::AddSphere(float radius, vec3 pos)
{
	pSphere* sph = new pSphere(radius, 12, 24);
	sph->SetPos(pos.x, pos.y, pos.z);

	if (wframe == true)
	{
		sph->wire = true;
	}

	sceneObjects.push_back(sph);

	App->physics->AddBody(*sph);
}

void ModuleSceneEditor::AddPlane(float x, float y, float z, float d, vec3 pos)
{
	pPlane* pl = new pPlane(x, y, z, d);
	pl->constant = d;
	
	pl->SetPos(pos.x, pos.y, pos.z);

	if (wframe == true)
	{
		pl->wire = true;
	}

	sceneObjects.push_back(pl);
}

void ModuleSceneEditor::AddPlaneNoGrid(float x, float y, float z, float d, vec3 pos)
{
	PlaneNoGrid* plN = new PlaneNoGrid(x, y, z, d);
	plN->constant = d;

	plN->SetPos(pos.x, pos.y, pos.z);

	if (wframe == true)
	{
		plN->wire = false;
	}

	sceneObjects.push_back(plN);
}

void ModuleSceneEditor::AddCapsule(float radius, float height, vec3 pos)
{
	pCapsule* cap = new pCapsule;
	cap->radius = radius;
	cap->height = height;
	cap->SetPos(pos.x, pos.y, pos.z);

	if (wframe == true)
	{
		cap->wire = true;
	}

	sceneObjects.push_back(cap);
}

GameObject* ModuleSceneEditor::CreateNewGameObject(const char* path)
{
	GameObject* ret = App->importer->LoadGameObject(path);
	root->DeleteChilds();
	root->AddChild(ret);

	return ret;
}