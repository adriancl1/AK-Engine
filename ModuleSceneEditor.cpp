#include "Application.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ModuleSceneEditor.h"
#include "Quadtree.h"

#include "Glew\include\glew.h"
#include "MathGeo\Geometry\Triangle.h"
#include "MathGeo\Math\float4x4.h"
#include "MathGeo\Geometry\LineSegment.h"
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

bool ModuleSceneEditor::Init(Configuration data)
{
	BROFILER_CATEGORY("Module SceneEditor Start", Profiler::Color::AliceBlue);

	saveLoadTimer.Start();

	return true;
}

bool ModuleSceneEditor::Start()
{
	//App->audio->PlayMusic("audio/walkwithme.ogg");
	App->camera->Move(vec3(0, 1, 0));

	root = new GameObject();
	root->SetName("Root");
	GameObject* scene = new GameObject();
	root->AddChild(scene);
	scene->SetName("Scene");

	ComponentCamera* camera = new ComponentCamera();

	App->camera->SetMainCamera(camera);

	scene->AddComponent(camera);

	tree = new Quadtree(); 

	return true;
}
bool ModuleSceneEditor::CleanUp(Configuration data)
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
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && root->GetChilds().empty() != true)
	{
		App->camera->CenterToGO(root->GetChilds()[0]);
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		wantToLoad = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		wantToSave = true;
	}

	return UPDATE_CONTINUE;
}
update_status ModuleSceneEditor::PostUpdate(float dt)
{
	if (wantToLoad == true)
	{
		LOG("Loading scene.");
		saveLoadTimer.Start();
		LoadScene(loadPath.c_str());
		wantToLoad = false;
		loadPath.clear();
	}
	else if (wantToSave == true)
	{
		LOG("Saving scene.");
		saveLoadTimer.Start();
		SaveScene("TestScene");
		LOG("Save completed in %i ms", saveLoadTimer.Read());
		saveLoadTimer.Stop();
		wantToSave = false;
	}
	if (recalcTree == true)
	{
		RecalculateQuadtree();
	}
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

void ModuleSceneEditor::SelectGameObject(LineSegment& picking)
{
	std::vector<GameObject*> aabbIntersections;

	root->CollectIntersectionsAABB(aabbIntersections, picking);

	if (!aabbIntersections.empty())
	{
		GameObject* closest = nullptr;
		float prevDistance = 30000;
		for (int i = 0; i < aabbIntersections.size(); i++)
		{
			float distance = 35000;
			float3 hitPoint;
			aabbIntersections[i]->CollectTriIntersections(picking, distance, hitPoint);
			if (distance < prevDistance)
			{
				prevDistance = distance;
				closest = aabbIntersections[i];
			}
		}

		if (closest != nullptr)
		{
			SetSelected(closest);
		}
		else
		{
			SetSelected(nullptr);
		}
	}
}

void ModuleSceneEditor::ShowEditor()
{
	root->OnEditor();

	if (selected != nullptr)
	{
		selected->ShowProperties();
	}
}

//TODO: Remove this
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
		if (selected != nullptr)
		{
			selected->selected = true;
		}
		this->selected = selected;
	}
	else
	{
		if (this->selected != nullptr)
		{
			this->selected->selected = false;
			this->selected = nullptr;
		}
	}
}

void ModuleSceneEditor::InsertToQuadtree(GameObject * toAdd)
{
	tree->Insert(toAdd);
}

void ModuleSceneEditor::RecalculateQuadtree()
{
	delete tree;
	tree = new Quadtree();
	root->InsertSelfAndChilds();
	recalcTree = false;
}

GameObject* ModuleSceneEditor::CreateNewGameObject(const char* path)
{
	GameObject* ret = App->importer->LoadGameObject(path);
	if (ret != nullptr)
	{
		root->AddChild(ret);
		ret->UpdateChildsTransform();
		App->camera->CenterToGO(ret);
		root->InsertSelfAndChilds();
	}
	else
	{
		LOG("Couldn't load .fbx file!");
	}

	return ret;
}

void ModuleSceneEditor::CreateEmptyGameObject()
{
	GameObject* newGO = new GameObject();
	ComponentTransform* newTrans = new ComponentTransform();
	newGO->AddComponent(newTrans);
	if (selected != nullptr)
	{
		selected->AddChild(newGO);
		newTrans->UpdateTrans();
	}
	else
	{
		root->AddChild(newGO);
	}
}

void ModuleSceneEditor::WantToLoadScene(const char * fileTitle)
{
	wantToLoad = true;
	loadPath = fileTitle;
}

void ModuleSceneEditor::SaveScene(const char* fileTitle) const
{
	Configuration save;
	save.AddArray("Scene Resources");
	App->resources->SaveResources(save);
	save.AddArray("Scene Game Objects");

	root->OnSerialize(save);

	char* buffer = nullptr;
	uint fileSize = save.SaveFile(&buffer, "Scene save");
	App->fileSystem->SaveFile(fileTitle, buffer, fileSize, FileType::fileScene);

	RELEASE_ARRAY(buffer);
}

void ModuleSceneEditor::LoadScene(const char * fileTitle)
{
	Configuration load(fileTitle);

	if (load.IsValueValid() == true)
	{
		root->DeleteChilds();
		selected = nullptr;
		App->camera->SetMainCamera(nullptr);
		App->resources->LoadResources(load);
		for (int i = 0; i < load.GetArraySize("Scene Game Objects"); i++)
		{
			GameObject* tmp = new GameObject();
			Configuration testC = load.GetArray("Scene Game Objects", i);
			tmp->OnDeserialize(testC);
		}
		LOG("Load completed in %i ms", saveLoadTimer.Read());
		root->UpdateChildsTransform();
		saveLoadTimer.Stop();
	}
	else
	{
		LOG("Scene file not valid.");
	}
}
