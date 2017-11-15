#include "Globals.h"
#include "Application.h"
#include "Brofiler-1.1.2\Brofiler.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "camera";

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	editorCamera = new ComponentCamera();
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp(Configuration data)
{
	LOG("Cleaning camera");
	if (editorCamera != nullptr)
	{
		delete editorCamera;
		editorCamera = nullptr;
	}
	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	BROFILER_CATEGORY("Module Camera Update", Profiler::Color::AliceBlue);

	vec3 newPos(0,0,0);
	float speed = 15.0f * dt;
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		if (UsingSceneCamera() == false)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
			{
				if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				{
					speed = 200.0f * dt;
				}

				if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
				if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
				if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

				Position += newPos;
				Reference += newPos;
			}

			// Mouse motion ----------------

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && (App->input->GetKey(SDL_SCANCODE_LALT) || App->input->GetKey(SDL_SCANCODE_RALT)))
			{
				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();

				float Sensitivity = 0.25f;

				Position -= Reference;

				if (dx != 0)
				{
					float DeltaX = (float)dx * Sensitivity;

					X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				}

				if (dy != 0)
				{
					float DeltaY = (float)dy * Sensitivity;

					Y = rotate(Y, DeltaY, X);
					Z = rotate(Z, DeltaY, X);

					if (Y.y < 0.0f)
					{
						Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
						Y = cross(Z, X);
					}
				}

				Position = Reference + Z * length(Position);
			}//App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RALT) == KEY_IDLE
			else if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
			{
				LineSegment picking = editorCamera->GetFrustum().UnProjectLineSegment(-(1 - App->input->GetNormalizedMouseX() * 2), (1 -App->input->GetNormalizedMouseY() * 2));
				lastPick = picking;

				App->sceneEditor->SelectGameObject(picking);
			}
			editorCamera->UpdateCamera(float3(Position.x, Position.y, Position.z), -float3(Z.x, Z.y, Z.z), float3(Y.x, Y.y, Y.z));
			CalculateViewMatrix();
		}
	}

	// Recalculate matrix -------------
	

	pLine a(lastPick.a.x, lastPick.a.y, lastPick.a.z, lastPick.b.x, lastPick.b.y, lastPick.b.z);
	a.color = Red;
	a.Render();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

void ModuleCamera3D::ZoomIn()
{
	vec3 newPos(0, 0, 0);
	newPos -= Z;

	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::ZoomOut()
{
	vec3 newPos(0, 0, 0);
	newPos += Z;

	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::CenterToGO(GameObject * centerTo)
{
	ComponentMesh* mesh = dynamic_cast<ComponentMesh*>(centerTo->FindComponent(Component_Mesh));
	if (mesh != nullptr)
	{
		float3 temp = mesh->GetCenter();
		float3 size = mesh->GetEnclosingBox().Size() * 0.5f;
		Position = vec3(temp.x + size.x, temp.y + size.y, temp.z + 5.0f + size.z);
		Reference = vec3(temp.x + size.x, temp.y + size.y, temp.z + size.z);
		LookAt(vec3(temp.x, temp.y, temp.z));
	}
}

void ModuleCamera3D::SetMainCamera(ComponentCamera * camera)
{
	mainCamera = camera;
}

void ModuleCamera3D::RecalcAspectRatio(int width, int height)
{
	editorCamera->SetAspectRatio(width, height);
}

bool ModuleCamera3D::UsingSceneCamera() const
{
	return mainCamera != nullptr && mainCamera->IsActive();
}

ComponentCamera * ModuleCamera3D::GetEditorCamera() const
{
	return editorCamera;
}

ComponentCamera * ModuleCamera3D::GetMainCamera() const
{
	return mainCamera;
}

// -----------------------------------------------------------------
const float* ModuleCamera3D::GetViewMatrix() const
{
	if (mainCamera != nullptr && mainCamera->IsActive())
	{
		return mainCamera->GetViewMatrix();
	}
	else
	{
		//return &ViewMatrix;
		return editorCamera->GetViewMatrix();
	}
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}