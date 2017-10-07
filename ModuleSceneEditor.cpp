#include "Application.h"
#include "Glew\include\glew.h"
#include "MathGeo\Geometry\Triangle.h"
#include "MathGeo\Math\float4x4.h"
#include "Brofiler-1.1.2\Brofiler.h"
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

	for (std::list<Mesh*>::iterator it = sceneMeshes.begin(); it != sceneMeshes.end(); ++it)
	{
		App->renderer3D->Draw((*it));
	}

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

void ModuleSceneEditor::AddMesh(Mesh* newMesh)
{
	sceneMeshes.push_back(newMesh);
}

void Mesh::DrawDebug()
{
	if (idNormals>0)
	{
		for (int i = 0; i < numVertices; i += 3)
		{
			pLine n(vertices[i], vertices[i + 1], vertices[i + 2], normals[i] + vertices[i], normals[i + 1] + vertices[i + 1], normals[i + 2] + vertices[i + 2]);
			n.color = Green;
			n.Render();
		}
		for (int i = 0; i < numVertices; i += 9)
		{
			Triangle face(float3(vertices[i], vertices[i + 1], vertices[i + 2]), float3(vertices[i + 3], vertices[i + 4], vertices[i + 5]), float3(vertices[i + 6], vertices[i + 7], vertices[i + 8]));
			float3 faceCenter = face.Centroid();
			float3 faceNormal = face.NormalCCW();
			pLine normal(faceCenter.x, faceCenter.y, faceCenter.z, faceCenter.x + faceNormal.x, faceCenter.y + faceNormal.y, faceCenter.z + faceNormal.z);
			normal.color = Orange;
			normal.Render();
		}
	}

	//Draw enclosing box ----
		float3 corners[8];
		enclosingBox.GetCornerPoints(corners);

		glPushMatrix();

		glMultMatrixf((GLfloat*)float4x4::identity.Transposed().ptr());
		glColor3f(Azure.r, Azure.g, Azure.b);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_QUADS);

		glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy, sz);
		glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy, sz);
		glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx,  sy, sz);
		glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy, sz);

		glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);
		glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx,  sy, -sz);

		glVertex3fv((GLfloat*)&corners[5]); //glVertex3f(sx, -sy,  sz);
		glVertex3fv((GLfloat*)&corners[4]); //glVertex3f(sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[6]); //glVertex3f(sx,  sy, -sz);
		glVertex3fv((GLfloat*)&corners[7]); //glVertex3f(sx,  sy,  sz);

		glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);
		glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy,  sz);
		glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);

		glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx, sy,  sz);
		glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx, sy,  sz);
		glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx, sy, -sz);
		glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx, sy, -sz);

		glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy,  sz);
		glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);

		glEnd();
		glPopMatrix();

		glColor3f(0, 0, 0);

}