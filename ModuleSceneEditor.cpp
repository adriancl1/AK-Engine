#include "Application.h"
#include "ModuleSceneEditor.h"


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
}

bool ModuleSceneEditor::Init(JSON_Object* data)
{
	return true;
}

bool ModuleSceneEditor::Start()
{
	//App->audio->PlayMusic("audio/walkwithme.ogg");
	App->camera->Move(vec3(0, 1, 0));

	return true;
}
bool ModuleSceneEditor::CleanUp(JSON_Object* data)
{
	return true;
}

update_status ModuleSceneEditor::PreUpdate(float dt)
{
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

	Plane p(0, 0, 0, 100);
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
	Cylinder* cyl = new Cylinder;
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
	Sphere* sph = new Sphere(radius, 12, 24);
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
	Plane* pl = new Plane(x, y, z, d);
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
	Capsule* cap = new Capsule;
	cap->radius = radius;
	cap->height = height;
	cap->SetPos(pos.x, pos.y, pos.z);

	if (wframe == true)
	{
		cap->wire = true;
	}

	sceneObjects.push_back(cap);

}