#include "Application.h"
#include "GameObject.h"
#include "ModuleSceneEditor.h"

#include "Glew\include\glew.h"
#include "MathGeo\Geometry\Triangle.h"
#include "MathGeo\Math\float4x4.h"
#include "Brofiler-1.1.2\Brofiler.h"

#include "mmgr\mmgr.h"

ModuleSceneEditor::ModuleSceneEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	name = "Scene editor";
}
ModuleSceneEditor::~ModuleSceneEditor()
{

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
	return UPDATE_CONTINUE;
}
update_status ModuleSceneEditor::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && root->childs.empty() != true)
	{
		App->camera->CenterToGO(root->childs[0]);
	}
	return UPDATE_CONTINUE;
}
update_status ModuleSceneEditor::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleSceneEditor::Draw()
{
	root->Update();

	pPlane p(0, 0, 0, 100);
	p.color = White;
	p.Render();
}

void ModuleSceneEditor::ShowEditor()
{
	root->OnEditor();
}


GameObject* ModuleSceneEditor::GetRoot() const
{
	return root;
}

GameObject* ModuleSceneEditor::CreateNewGameObject(const char* path)
{
	GameObject* ret = App->importer->LoadGameObject(path);	
	if (ret != nullptr)
	{
		root->DeleteChilds();
		root->AddChild(ret);
		App->camera->CenterToGO(ret);
	}
	else
	{
		LOG("Couldn't load .fbx file!");
	}	

	return ret;
}