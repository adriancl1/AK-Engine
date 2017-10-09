#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ComponentMesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(JSON_Object* data = nullptr);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp(JSON_Object* data = nullptr);

	void OnResize(int width, int height);

	void SetDepthTest();
	void SetCullFace();
	void SetLighting();
	void SetColorMaterial();
	void SetTexture2D();

	void Draw(ComponentMesh* toDraw);
	uint imageLoaded;

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool depthTest;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
};

#endif //__ModuleRenderer_H__