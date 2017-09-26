#include "Application.h"
#include "ModuleSceneEditor.h"


ModuleSceneEditor::ModuleSceneEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	name = "Scene editor";
}
ModuleSceneEditor::~ModuleSceneEditor()
{
	while (!sceneCubes.empty())
	{
		delete sceneCubes.front();
		sceneCubes.pop_front();
	}
	while (!sceneCylinders.empty())
	{
		delete sceneCylinders.front();
		sceneCylinders.pop_front();
	}
	while (!sceneSpheres.empty())
	{
		delete sceneSpheres.front();
		sceneSpheres.pop_front();
	}
}

bool ModuleSceneEditor::Init(JSON_Object* data)
{
	return true;
}

bool ModuleSceneEditor::Start()
{
	App->audio->PlayMusic("audio/walkwithme.ogg");
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
	for (std::list<Cube*>::iterator it = sceneCubes.begin(); it != sceneCubes.end(); ++it)
	{
		(*it)->Render();
	}
	for (std::list<Cylinder*>::iterator it = sceneCylinders.begin(); it != sceneCylinders.end(); ++it)
	{
		(*it)->Render();
	}
	for (std::list<Sphere*>::iterator it = sceneSpheres.begin(); it != sceneSpheres.end(); ++it)
	{
		(*it)->Render();
	}

	Plane p(0, 0, 0, 100);
	p.color = White;
	p.Render();
}

void ModuleSceneEditor::SetToWireframe(bool wframe)
{
	if (wframe == true)
	{
		for (std::list<Cube*>::iterator it = sceneCubes.begin(); it != sceneCubes.end(); ++it)
		{
			(*it)->wire = true;
		}
		for (std::list<Cylinder*>::iterator it = sceneCylinders.begin(); it != sceneCylinders.end(); ++it)
		{
			(*it)->wire = true;
		}
		for (std::list<Sphere*>::iterator it = sceneSpheres.begin(); it != sceneSpheres.end(); ++it)
		{
			(*it)->wire = true;
		}
	}
	else
	{
		for (std::list<Cube*>::iterator it = sceneCubes.begin(); it != sceneCubes.end(); ++it)
		{
			(*it)->wire = false;
		}
		for (std::list<Cylinder*>::iterator it = sceneCylinders.begin(); it != sceneCylinders.end(); ++it)
		{
			(*it)->wire = false;
		}
		for (std::list<Sphere*>::iterator it = sceneSpheres.begin(); it != sceneSpheres.end(); ++it)
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

	sceneCubes.push_back(cube);

	App->physics->AddBody(*cube);
}

void ModuleSceneEditor::AddCylinder(float radius, float height, vec3 pos)
{
	Cylinder* cyl = new Cylinder;
	cyl->radius = radius;
	cyl->height = height;
	cyl->SetPos(pos.x, pos.y, pos.z);

	sceneCylinders.push_back(cyl);

	App->physics->AddBody(*cyl);
}

void ModuleSceneEditor::AddSphere(float radius, vec3 pos)
{
	Sphere* sph = new Sphere;
	sph->radius = radius;
	sph->SetPos(pos.x, pos.y, pos.z);

	sceneSpheres.push_back(sph);

	App->physics->AddBody(*sph);
}