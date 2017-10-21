#include "Application.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ModuleSceneEditor.h"
#include "Quadtree.h"

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
	root->SetName("Scene");

	ComponentCamera* camera = new ComponentCamera();

	root->AddComponent(camera);

	tree = new Quadtree(AABB(float3(-100, -5, -100), float3(100, 5, 100))); 

	return true;
}
bool ModuleSceneEditor::CleanUp(JSON_Object* data)
{
	delete root;
	return true;
}

update_status ModuleSceneEditor::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}
update_status ModuleSceneEditor::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && root->childs.empty() != true)
	{
		App->camera->CenterToGO(root->childs[0]);
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		//ComponentMesh* m = new ComponentMesh();
		//App->importer->Load(m, "hola.T");
		//root->AddComponent(m);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		if (root->childs.size() != 0)
		{
			//App->importer->Save((ComponentMesh*)root->childs[0]->FindComponent(Component_Mesh), "hola.T");
		}
	}

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

	if (App->physics->debug)
	{
		tree->DrawDebug(Cyan); 
	}

	pPlane p(0, 0, 0, 100);
	p.color = White;
	p.Render();
}

void ModuleSceneEditor::ShowEditor()
{
		root->OnEditor();

		if (selected != nullptr)
		{
			selected->ShowProperties();
		}
	
}

void ModuleSceneEditor::AddCube(vec3 size, vec3 pos)
{
	Cube* cube = new Cube;
	cube->size.Set(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);

	sceneObjects.push_back(cube);

	App->physics->AddBody(*cube);
}

void ModuleSceneEditor::AddCube1(vec3 size, vec3 pos)
{
	Cube1* cube = new Cube1(size.x, size.y, size.z);
	cube->size.Set(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);

	sceneObjects.push_back(cube);

	App->physics->AddBody(*cube);
}

void ModuleSceneEditor::AddCube2(vec3 size, vec3 pos)
{
	Cube2* cube = new Cube2(size.x, size.y, size.z);
	cube->size.Set(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);

	sceneObjects.push_back(cube);

	App->physics->AddBody(*cube);
}

void ModuleSceneEditor::AddCylinder(float radius, float height, vec3 pos)
{
	pCylinder* cyl = new pCylinder;
	cyl->radius = radius;
	cyl->height = height;
	cyl->SetPos(pos.x, pos.y, pos.z);

	sceneObjects.push_back(cyl);

	App->physics->AddBody(*cyl);
}

void ModuleSceneEditor::AddSphere(float radius, vec3 pos)
{
	pSphere* sph = new pSphere(radius, 12, 24);
	sph->SetPos(pos.x, pos.y, pos.z);

	sceneObjects.push_back(sph);

	App->physics->AddBody(*sph);
}

void ModuleSceneEditor::AddPlane(float x, float y, float z, float d, vec3 pos)
{
	pPlane* pl = new pPlane(x, y, z, d);
	pl->constant = d;
	
	pl->SetPos(pos.x, pos.y, pos.z);

	sceneObjects.push_back(pl);
}

void ModuleSceneEditor::AddPlaneNoGrid(float x, float y, float z, float d, vec3 pos)
{
	PlaneNoGrid* plN = new PlaneNoGrid(x, y, z, d);
	plN->constant = d;

	plN->SetPos(pos.x, pos.y, pos.z);

	sceneObjects.push_back(plN);
}

void ModuleSceneEditor::AddCapsule(float radius, float height, vec3 pos)
{
	pCapsule* cap = new pCapsule;
	cap->radius = radius;
	cap->height = height;
	cap->SetPos(pos.x, pos.y, pos.z);

	sceneObjects.push_back(cap);
}

GameObject* ModuleSceneEditor::GetRoot()
{
	return root;
}

Quadtree * ModuleSceneEditor::GetQuadtree()
{
	return tree;
}

void ModuleSceneEditor::SetSelected(GameObject * selected)
{
	if (this->selected != selected)
	{
		if (this->selected != nullptr)
		{
			this->selected->selected = false;
		}
		selected->selected = true;
		this->selected = selected;
	}
	else
	{
		this->selected->selected = false;
		this->selected = nullptr;
	}
}

GameObject* ModuleSceneEditor::CreateNewGameObject(const char* path)
{
	GameObject* ret = App->importer->LoadGameObject(path);
	if (ret != nullptr)
	{
		//root->DeleteChilds();
		root->AddChild(ret);
		App->camera->CenterToGO(ret);
	}
	else
	{
		LOG("Couldn't load .fbx file!");
	}

	return ret;
}