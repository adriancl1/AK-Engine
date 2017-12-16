#include "Application.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ModuleSceneEditor.h"
#include "Quadtree.h"
#include "ModuleRenderer3D.h"

#include "Glew/include/glew.h"
#include "MathGeo/Geometry/Triangle.h"
#include "MathGeo/Math/float4x4.h"
#include "MathGeo/Geometry/LineSegment.h"
#include "Brofiler-1.1.2/Brofiler.h"
#include "mmgr/mmgr.h"

ModuleSceneEditor::ModuleSceneEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	name = "Scene editor";
}
ModuleSceneEditor::~ModuleSceneEditor()
{

}

bool ModuleSceneEditor::Init(Configuration data)
{
	BROFILER_CATEGORY("Module SceneEditor Start", Profiler::Color::AliceBlue);

	saveLoadTimer.Start();

	return true;
}

bool ModuleSceneEditor::Start()
{
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
	delete tree;
	return true;
}

update_status ModuleSceneEditor::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}
update_status ModuleSceneEditor::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && selected != nullptr)
	{
			ComponentMesh* mesh = (ComponentMesh*)selected->FindComponent(Component_Mesh);
			if (mesh != nullptr)
			{
				AABB box = mesh->GetEnclosingBox();
				ComponentTransform* trans = (ComponentTransform*)selected->FindComponent(Component_Transform);
				box.TransformAsAABB(trans->GetGlobalTransform());
				App->camera->CenterToGO(box);
			}
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		wantToLoad = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		wantToSave = true;
	}

	root->Update();

	return UPDATE_CONTINUE;
}
update_status ModuleSceneEditor::PostUpdate(float dt)
{
	root->PostUpdate();

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
		SaveScene("MainScene");
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
	if (App->quadtreeAcceleration == true && App->camera->GetMainCamera()->GetFrustumCulling() == true)
	{
		std::vector<GameObject*> toDraw;
		tree->CollectIntersections(toDraw, App->camera->GetMainCamera()->GetFrustum());

		for (int i = 0; i < toDraw.size(); i++)
		{
			App->renderer3D->Draw(toDraw[i]);
		}
	}
	else
	{
		root->RecursiveDraw();
	}

	if (App->GetShowQuadtree())
	{
		tree->DrawDebug(Cyan); 
	}

	pPlane p(0, 0, 0, 100);
	p.color = White;
	p.Render();
}

void ModuleSceneEditor::SelectGameObject(LineSegment& picking)
{
	Timer tmpTimer;
	std::vector<GameObject*> aabbIntersections;
	if (App->quadtreeAcceleration == true)
	{
		if (App->performanceTimers == true)
		{
			LOG("Starting timer for mouse picking with quadtree acceleration");
			tmpTimer.Start();
		}
		std::vector<GameObject*> quadtreeIntersections;
		tree->CollectIntersections(quadtreeIntersections, picking);
		for (int i = 0; i < quadtreeIntersections.size(); i++)
		{
			if (quadtreeIntersections[i]->Intersects(picking) == true)
			{
				aabbIntersections.push_back(quadtreeIntersections[i]);
			}
		}
	}
	else
	{
		if (App->performanceTimers == true)
		{
			LOG("Starting timer for mouse picking without quadtree acceleration");
			tmpTimer.Start();
		}
		root->CollectAllIntersectionsAABB(aabbIntersections, picking);
	}
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
	else
	{
		SetSelected(nullptr);
	}
	if (App->performanceTimers == true)
	{
		LOG("Picking ended in %i ms", tmpTimer.Read());
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

GameObject* ModuleSceneEditor::GetRoot()
{
	return root;
}

Quadtree * ModuleSceneEditor::GetQuadtree()
{
	return tree;
}

GameObject * ModuleSceneEditor::GetSelected() const
{
	return selected;
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
		root->InsertSelfAndChilds();
		App->camera->CenterToGO(tree->root->box);
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
