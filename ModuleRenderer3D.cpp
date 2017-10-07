#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneEditor.h"
#include "Primitive.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include "Brofiler-1.1.2\Brofiler.h"
#include "Devil\include\ilut.h"
#include "Devil\include\il.h"
#include <gl/GL.h>
#include <gl/GLU.h>


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */
#pragma comment( lib, "Devil/libx86/DevIL.lib" ) /* Loading Devil lib */
#pragma comment( lib, "Devil/libx86/ILU.lib" ) /* Loading ILU lib */
#pragma comment( lib, "Devil/libx86/ILUT.lib" ) /* Loading ILUT lib */


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "renderer";
	depthTest = true;
	cullFace = true;
	lighting = true;
	colorMaterial = true;
	texture2D = true;
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

	glLineWidth(2.0f);
	imageLoaded = ImportImage("Assets/Lenna.png");

	GLubyte checkImage[128][128][4];
	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 128; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	uint imageName = 0;

	/*glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &imageName);
	glBindTexture(GL_TEXTURE_2D, imageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);*/


	glBegin(GL_TRIANGLES);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, imageLoaded);

	//Face 1 LEFT
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, 1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 1.f, 1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);


	//Face 2 FRONT
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);

	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 0.f, 0.f);
	
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	

	//Face 3 RIGHT
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 0.f, 1.f);
	
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 0.f, 0.f);
	
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 1.f);
	
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 0.f, 1.f);

	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	

	//Face 4
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 1.f, 1.f);

	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, 1.f);
	
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 0.f, 1.f);
	
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 0.f, 1.f);
	
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 1.f, 1.f);
	
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 1.f, 1.f);
	

	//Face 5
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 1.f);

	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);

	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, 0.f);

	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, 0.f);

	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 1.f, 1.f);

	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 1.f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glLineWidth(1.0f);

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

void ModuleRenderer3D::Draw(Mesh* toDraw)
{
	if (App->sceneEditor->GetWireframe() == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
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

	if (App->physics->debug)
	{
		toDraw->DrawDebug();
	}
	
}

uint ModuleRenderer3D::ImportImage(const char * image)
{

	ILuint imageID;				// Create an image ID as a ULuint

	GLuint textureID;			// Create a texture ID as a GLuint

	ILboolean success;			// Create a flag to keep track of success/failure

	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID

	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage("Assets/Lenna.png"); 	// Load the image file

											// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			LOG("Image conversion failed - IL reports error: %s ",iluErrorString(error));
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureID);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself
	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		LOG("Image load failed - IL reports error: %s", iluErrorString(error));
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	LOG("Texture creation successful." );

	return textureID; // Return the GLuint to the texture so you can use it!
}
