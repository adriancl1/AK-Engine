#include "Globals.h"
#include "Application.h"
#include "Primitive.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneEditor.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include "Brofiler-1.1.2\Brofiler.h"
#include "Devil\include\ilut.h"
#include "Devil\include\il.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "mmgr\mmgr.h"

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
bool ModuleRenderer3D::Init(JSON_Object* data)
{
	BROFILER_CATEGORY("Module Render Init", Profiler::Color::AliceBlue);

	LOG("Creating 3D Renderer context");
	bool ret = true;

	if (data != nullptr)
	{
		depthTest = json_object_dotget_boolean(data, "depthTest");
		cullFace = json_object_dotget_boolean(data, "cullFace");
		lighting = json_object_dotget_boolean(data, "lighting");
		depthTest = json_object_dotget_boolean(data, "depthTest");
		colorMaterial = json_object_dotget_boolean(data, "colorMaterial");
		texture2D = json_object_dotget_boolean(data, "texture2D");
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
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());
	
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
bool ModuleRenderer3D::CleanUp(JSON_Object* data)
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	JSON_Object* rendererData = json_object_dotget_object(data, name.c_str());

	json_object_dotset_boolean(rendererData, "depthTest", depthTest);
	json_object_dotset_boolean(rendererData, "cullFace", cullFace);
	json_object_dotset_boolean(rendererData, "lighting", lighting);
	json_object_dotset_boolean(rendererData, "colorMaterial", colorMaterial);
	json_object_dotset_boolean(rendererData, "texture2D", texture2D);

	return true;
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
	for (int i = 0; i < objectDraw->components.size(); i++)
	{
		if (objectDraw->components[i]->type == Component_Mesh)
		{
			if (wframe == true)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			ComponentMesh* toDraw = dynamic_cast<ComponentMesh*> (objectDraw->components[i]);
			glPushMatrix();

			//float m[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

			//glMultMatrixf(m);

			if (toDraw->idNormals > 0)
			{
				glEnable(GL_LIGHTING);
				glEnableClientState(GL_NORMAL_ARRAY);

				glBindBuffer(GL_ARRAY_BUFFER, toDraw->idNormals);
				glNormalPointer(GL_FLOAT, 0, NULL);
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, toDraw->idVertices);
			glVertexPointer(3, GL_FLOAT, 0, NULL);


			if (toDraw->idTexCoords > 0)
			{
				ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(objectDraw->FindComponent(Component_Material));
				if(mat!=nullptr)
				{
					glBindTexture(GL_TEXTURE_2D, mat->idTexture);
				}
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, toDraw->idTexCoords);
				glTexCoordPointer(3, GL_FLOAT, 0, NULL);
			}

			if (toDraw->idColors > 0)
			{
				glEnableClientState(GL_COLOR_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, toDraw->idColors);
				glColorPointer(3, GL_FLOAT, 0, NULL);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, toDraw->idIndices);
			glDrawElements(GL_TRIANGLES, toDraw->numIndices, GL_UNSIGNED_INT, NULL);


			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);

			glPopMatrix();
			glUseProgram(0);

			glBindTexture(GL_TEXTURE_2D, 0);

			if (App->physics->debug)
			{
				toDraw->DrawDebug();
			}

		}
	}
}

