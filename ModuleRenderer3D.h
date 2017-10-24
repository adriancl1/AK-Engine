#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ComponentMesh;
class GameObject;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(Configuration data);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp(Configuration data);

	void OnConfiguration()override;
	void OnResize(int width, int height);

	void SetDepthTest();
	void SetCullFace();
	void SetLighting();
	void SetColorMaterial();
	void SetTexture2D();

	bool GetWireframe()const;

	void Draw(GameObject* toDraw);
	void DrawMesh(ComponentMesh* toDraw);
	uint imageLoaded;

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

private:
	bool depthTest;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	bool wframe;
};

#endif //__ModuleRenderer_H__