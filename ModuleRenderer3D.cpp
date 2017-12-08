#include "Globals.h"
#include "Application.h"
#include "Primitive.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ResourceMesh.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneEditor.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "Brofiler-1.1.2/Brofiler.h"
#include "Devil/include/ilut.h"
#include "Devil/include/il.h"
#include <gl/GL.h>
#include <gl/GLU.h>


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Devil/libx86/DevIL.lib" ) /* Loading Devil lib */
#pragma comment (lib, "Devil/libx86/ILU.lib" ) /* Loading ILU lib */
#pragma comment (lib, "Devil/libx86/ILUT.lib" ) /* Loading ILUT lib */


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "renderer";
	depthTest = true;
	cullFace = true;
	lighting = true;
	colorMaterial = true;
	texture2D = true;
	wframe = false;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(Configuration data)
{
	BROFILER_CATEGORY("Module Render Init", Profiler::Color::AliceBlue);

	LOG("Creating 3D Renderer context");
	bool ret = true;

	if (!App->UseDefaultValues())
	{
		depthTest = data.GetBool("depthTest");
		cullFace = data.GetBool("cullFace");
		lighting = data.GetBool("lighting");
		depthTest = data.GetBool("depthTest");
		colorMaterial = data.GetBool("colorMaterial");
		texture2D = data.GetBool("texture2D");
	}
	
	//Set Attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


	//Create context
	context = SDL_GL_CreateContext(App->window->GetWindow());
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
		{
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (depthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		if (cullFace)
		{
			glEnable(GL_CULL_FACE);
		}
		lights[0].Active(true);
		if (lighting)
		{
			glEnable(GL_LIGHTING);
		}
		if (colorMaterial)
		{
			glEnable(GL_COLOR_MATERIAL);
		}
		if (texture2D)
		{
			glEnable(GL_TEXTURE_2D);
		}
	}

	// Projection matrix for
	int width, height;
	App->window->GetWindowSize(width, height);
	OnResize(width, height);
	imageLoaded = 0;

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Module Renderer PreUpdate", Profiler::Color::AliceBlue);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(App->camera->GetViewMatrix());
	glMatrixMode(GL_MODELVIEW);	

	/*Old Camera View
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());*/
	
	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Module Renderer PostUpdate", Profiler::Color::AliceBlue);

	App->sceneEditor->Draw();

	App->imGui->Draw();

	SDL_GL_SwapWindow(App->window->GetWindow());
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp(Configuration data)
{
	bool ret = true;
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	ret = data.SetBool("depthTest", depthTest);
	ret = data.SetBool("cullFace", cullFace);
	ret = data.SetBool("lighting", lighting);
	ret = data.SetBool("colorMaterial", colorMaterial);
	ret = data.SetBool("texture2D", texture2D);

	return ret;
}


void ModuleRenderer3D::OnConfiguration()
{
	if (ImGui::CollapsingHeader("Renderer"))
	{
		ImGui::Checkbox("Wireframe Mode", &wframe);
		if (ImGui::Checkbox("Depth Test", &depthTest))
		{
			SetDepthTest();
		}
		if (ImGui::Checkbox("Cull Face", &cullFace))
		{
			SetCullFace();
		}
		if (ImGui::Checkbox("Lighting", &lighting))
		{
			SetLighting();
		}
		if (ImGui::Checkbox("Color Material", &colorMaterial))
		{
			SetColorMaterial();
		}
		if (ImGui::Checkbox("2D Textures", &texture2D))
		{
			SetTexture2D();
		}
	}
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::SetDepthTest()
{
	if (depthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}
void ModuleRenderer3D::SetCullFace()
{
	if (cullFace)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}
void ModuleRenderer3D::SetLighting()
{
	if (lighting)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
}

void ModuleRenderer3D::SetColorMaterial()
{
	if (colorMaterial)
	{
		glEnable(GL_COLOR_MATERIAL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}
}

void ModuleRenderer3D::SetTexture2D()
{
	if (texture2D)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

bool ModuleRenderer3D::GetWireframe() const
{
	return wframe;
}

void ModuleRenderer3D::Draw(GameObject* objectDraw)
{
	ComponentTransform* tmpTrans = (ComponentTransform*)objectDraw->FindComponent(Component_Transform);
	ComponentCamera* camera = (ComponentCamera*)App->camera->GetMainCamera();

	glPushMatrix();
	if (tmpTrans != nullptr)
	{
		glMultMatrixf(tmpTrans->GetTransMatrix().Transposed().ptr());
	}

	for (int i = 0; i < objectDraw->GetComponents().size(); i++)
	{
		if (objectDraw->GetComponents()[i]->GetType() == Component_Mesh)
		{
			ComponentMesh* toDraw = dynamic_cast<ComponentMesh*> (objectDraw->GetComponents()[i]);
			if (camera != nullptr && camera->GetFrustumCulling() == true)
			{
				AABB recalculatedBox = toDraw->GetEnclosingBox();
				recalculatedBox.TransformAsAABB(tmpTrans->GetTransMatrix());
				if (camera->Contains(recalculatedBox))
				{
					DrawMesh(toDraw);
				}
			}
			else
			{
				DrawMesh(toDraw);
			}
		}	
	}
		glPopMatrix();
		glUseProgram(0);
}

void ModuleRenderer3D::DrawMesh(ComponentMesh * toDraw)
{
	if (toDraw->GetNumVertices() > 4)
	{
		if (wframe == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColor3f(0, 1, 1);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		if (toDraw->GetIDNormals() > 0)
		{
			glEnable(GL_LIGHTING);
			glEnableClientState(GL_NORMAL_ARRAY);

			if (toDraw->GetMeshDeformable() != nullptr)
			{
				glBindBuffer(GL_ARRAY_BUFFER, toDraw->GetMeshDeformable()->idNormals);
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER, toDraw->GetIDNormals());
			}		
			glNormalPointer(GL_FLOAT, 0, NULL);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);

		if (toDraw->GetMeshDeformable() != nullptr)
		{
			glBindBuffer(GL_ARRAY_BUFFER, toDraw->GetMeshDeformable()->idVertices);
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, toDraw->GetIDVertices());
		}
	
		glVertexPointer(3, GL_FLOAT, 0, NULL);


		if (toDraw->GetIDTextCoords() > 0)
		{
			ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(toDraw->GetGameObject()->FindComponent(Component_Material));
			if (mat != nullptr && wframe == false)
			{
				glBindTexture(GL_TEXTURE_2D, mat->GetTextureID());
			}
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, toDraw->GetIDTextCoords());
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		}

		if (toDraw->GetIDColors() > 0)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, toDraw->GetIDColors());
			glColorPointer(3, GL_FLOAT, 0, NULL);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, toDraw->GetIDIndices());
		glDrawElements(GL_TRIANGLES, toDraw->GetNumIndices(), GL_UNSIGNED_INT, NULL);


		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (App->GetDebug())
		{
			toDraw->DrawDebug();
		}
	}
}

