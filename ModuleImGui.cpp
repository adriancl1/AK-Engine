#include "Globals.h"
#include "Application.h"
#include "Brofiler-1.1.2\Brofiler.h"
#include "Math.h"
#include "ModuleImGui.h"
#include "imgui-1.51\imgui.h"
#include "imgui-1.51\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"


#pragma comment( lib, "Glew/libx86/glew32.lib" )

#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

//Displays usefull information about that option
static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{
}

//Load assets
bool ModuleImGui::Start()
{
	BROFILER_CATEGORY("Module ImGui Start", Profiler::Color::AliceBlue);

	LOG("Loading Intro assets");
	bool ret = true;

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->GetWindow());

	//ImGui Menu Active Booleans
	testWindowActive = false;
	menuActive = false;
	consoleActive = false;
	mathPlaygroundActive = false;
	configurationActive = false;
	aboutActive = false;
	createGeometryActive = false;
	editorActive = false;

	openMenuWindow = false;
	openConsoleWindow = false;
	openConfigurationWindow = false;
	openMathPlaygroundWindow = false;
	openAboutWindow = false;
	openCreateGeometryWindow = false;
	openEditorWindow = false;

	return ret;
}

//PreUpdate
update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());
	return(UPDATE_CONTINUE);
}

//Update
update_status ModuleImGui::Update(float dt)
{
	BROFILER_CATEGORY("Module ImGui Update", Profiler::Color::AliceBlue);

	//Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "ESC"))
			{
				return UPDATE_STOP;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console"))
			{
				openConsoleWindow = !openConsoleWindow;
				consoleActive = !consoleActive;
			}
			if (ImGui::MenuItem("Math Test Playground"))
			{
				openMathPlaygroundWindow = !openMathPlaygroundWindow;
				mathPlaygroundActive = !mathPlaygroundActive;
			}
			if (ImGui::MenuItem("Create Geometry"))
			{
				openCreateGeometryWindow = !openCreateGeometryWindow;
				createGeometryActive= !createGeometryActive;
			}
			if (ImGui::MenuItem("Configuration"))
			{
				openConfigurationWindow = !openConfigurationWindow;
				configurationActive = !configurationActive;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("ImGui Demo"))
			{
				testWindowActive = !testWindowActive;
			}
			if (ImGui::MenuItem("Documentation"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/blob/master/README.md", NULL, NULL, SW_SHOWNORMAL); // This should be the wiki when we have one.
			}
			if (ImGui::MenuItem("Latest Release"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/releases", NULL, NULL, SW_SHOWNORMAL); 
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/issues", NULL, NULL, SW_SHOWNORMAL); 
			}


			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	//-----
	ShowDebugWindow();

	if (menuActive)
	{
		ShowMenuWindow();
	}
	if (testWindowActive)
	{
		ImGui::ShowTestWindow();
	}
	if (consoleActive)
	{
		ShowConsoleWindow();
	}
	if (mathPlaygroundActive)
	{
		ShowMathWindow();
	}
	if (createGeometryActive)
	{
		ShowCreateGeometryWindow();
	}
	if (configurationActive)
	{
		ShowConfigurationWindow();
	}
	if (editorActive)
	{
		ShowEditorWindow();
	}
	if (aboutActive)
	{
		ShowAboutWindow();
	}

	if (closeApp)
	{
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp(JSON_Object* data)
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleImGui::ShowDebugWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Debug", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	if (ImGui::CollapsingHeader("Debug Options"))
	{
		if (ImGui::Checkbox("Show Menu", &openMenuWindow))
		{
			menuActive = !menuActive;
		}	
		if (ImGui::Checkbox("Show Console", &openConsoleWindow))
		{
			consoleActive = !consoleActive;
		}
		if (ImGui::Checkbox("Show Math Test Playground", &openMathPlaygroundWindow))
		{
			mathPlaygroundActive = !mathPlaygroundActive;
		}
		if (ImGui::Checkbox("Show Create Geometry", &openCreateGeometryWindow))
		{
			createGeometryActive = !createGeometryActive;
		}
		if (ImGui::Checkbox("Show Configuration", &openConfigurationWindow))
		{
			configurationActive = !configurationActive;
		}
		if (ImGui::Checkbox("Show Editor", &openEditorWindow))
		{
			editorActive = !editorActive;
		}
		if (ImGui::Checkbox("Show About..", &openAboutWindow))
		{
			aboutActive = !aboutActive;
		}
	}
	ImGui::End();
}

void ModuleImGui::Draw()const
{
	ImGui::Render();
}

void ModuleImGui::ShowMenuWindow(bool* p_open)
{
	static bool openTestWindow = false;

	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("ImGui Menu", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	ImGui::Text("ImGui Menu");

	if (ImGui::CollapsingHeader("Options"))
	{
		ImGui::TextWrapped("Options Menu");

		if (ImGui::Checkbox("Show Demo", &openTestWindow))
		{
			testWindowActive = !testWindowActive;
		}
	}

	if (ImGui::Button("Close App", ImVec2(300,20)))
	{
		closeApp = true;
	}
	ImGui::End(); 
}

void ModuleImGui::ShowConsoleWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Console", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	//ImGui::Text("%s", consoleText);

	if (ImGui::Button("Clear"))
	{
		consoleText.clear();
	}

	for (int i = consoleText.size() - 1; i >= 0; i--)
	{
		ImGui::Text("%s", consoleText[i].c_str());
	}

	ImGui::End();
}

void ModuleImGui::AddLogToWindow(std::string toAdd)
{
	consoleText.push_back(toAdd);
}

void ModuleImGui::ShowMathWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Math Library Tests", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	ImGui::Text("Fill the following parameters:");

	if (ImGui::CollapsingHeader("Intersection Sphere-Sphere"))
	{
		ImGui::Text("Sphere 1:");
		ImGui::InputInt("Radius", &sphereRadius);
		ImGui::InputInt("X", &sphereX);
		ImGui::InputInt("Y", &sphereY);
		ImGui::InputInt("Z", &sphereZ);

		ImGui::Text("Sphere 2:");
		ImGui::InputInt("Radius 2", &sphereRadius2);
		ImGui::InputInt("X 2", &sphereX2);
		ImGui::InputInt("Y 2", &sphereY2);
		ImGui::InputInt("Z 2", &sphereZ2);


		if (ImGui::Button("Run Math Test"))
		{
			math::Sphere sphere1(float3(sphereX, sphereY, sphereZ), sphereRadius);
			math::Sphere sphere2(float3(sphereX2, sphereY2, sphereZ2), sphereRadius2);

			intersects = sphere1.Intersects(sphere2);

			if (intersects)
			{
				intersectsTrue = true;
				intersectsFalse = false;
			}
			else
			{
				intersectsTrue = false;
				intersectsFalse = true;
			}
		}

		if (intersectsTrue)
		{
			ImGui::Text("True");
		}
		if (intersectsFalse)
		{
			ImGui::Text("False");
		}
	}

	if (ImGui::CollapsingHeader("Intersection Sphere-Capsule"))
	{
		ImGui::Text("Sphere 1:");
		ImGui::InputInt("Sphere 1 Radius", &sphereRadius);
		ImGui::InputInt("X", &sphereX);
		ImGui::InputInt("Y", &sphereY);
		ImGui::InputInt("Z", &sphereZ);

		ImGui::Text("Capsule 1:");
		ImGui::InputInt("Capsule 1 Radius", &capsuleRadius);
		ImGui::InputInt("Bottom X", &capsuleBotX);
		ImGui::InputInt("Bottom Y", &capsuleBotY);
		ImGui::InputInt("Bottom Z", &capsuleBotZ);
		ImGui::InputInt("Top X", &capsuleTopX);
		ImGui::InputInt("Top Y", &capsuleTopY);
		ImGui::InputInt("Top Z", &capsuleTopZ);

		if (ImGui::Button("Run Math Test"))
		{
			math::Sphere sphere1(float3(sphereX, sphereY, sphereZ), sphereRadius);
			math::Capsule capsule1(float3(capsuleBotX, capsuleBotY, capsuleBotZ), float3(capsuleTopX, capsuleTopY, capsuleTopZ), capsuleRadius);

			intersects = sphere1.Intersects(capsule1);

			if (intersects)
			{
				intersectsTrue = true;
				intersectsFalse = false;
			}
			else
			{
				intersectsTrue = false;
				intersectsFalse = true;
			}
		}
		if (intersectsTrue)
		{
			ImGui::Text("True");
		}
		if (intersectsFalse)
		{
			ImGui::Text("False");
		}
	}

	if (ImGui::Button("Reset"))
	{
		//Properties Sphere 1
		sphereRadius = 0;
		sphereX = 0;
		sphereY = 0;
		sphereZ = 0;

		//Sphere 2
		sphereRadius2 = 0;
		sphereX2 = 0;
		sphereY2 = 0;
		sphereZ2 = 0;

		//Capsule 1
		capsuleRadius = 0;
		capsuleBotX = 0;
		capsuleBotY = 0;
		capsuleBotZ = 0;
		capsuleTopX = 0;
		capsuleTopY = 0;
		capsuleTopZ = 0;

		//Booleans
		intersects = false;
		intersectsTrue = false;
		intersectsFalse = false;
	}


	ImGui::End();
}


void ModuleImGui::ShowConfigurationWindow(bool* p_open)
{
	if (!ImGui::Begin("Configuration", p_open))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	ImGui::Text("Options");

	App->OnConfiguration();

	ImGui::End();
}

void ModuleImGui::ShowAboutWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("About", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

										
	ImGui::Text("Name: AK Engine.");
	ImGui::Text("3D Engine made with C++ and OpenGL for an assignment in a Game Design & Development degree.");
	ImGui::Text("Authors: Marc Fabian, Adrian Castillo and Marc Lopez.");
	
	if (ImGui::CollapsingHeader("Libraries used"))
	{
		if (ImGui::MenuItem("SDL 2.0.4"))
		{
			ShellExecuteA(NULL, "open", "https://www.libsdl.org/index.php", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("MathGeoLib 1.3"))
		{
			ShellExecuteA(NULL, "open", "http://clb.demon.fi/MathGeoLib/nightly/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("ImGui 1.52 WIP"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("Bullet 2.84"))
		{
			ShellExecuteA(NULL, "open", "http://bulletphysics.org/wordpress/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("Brofiler 1.1.2"))
		{
			ShellExecuteA(NULL, "open", "http://brofiler.com/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("Glew 2.0.0"))
		{
			ShellExecuteA(NULL, "open", "http://glew.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("OpenGl 3.1"))
		{
			ShellExecuteA(NULL, "open", "https://www.opengl.org/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("DevIL 1.8.0"))
		{
			ShellExecuteA(NULL, "open", "http://openil.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("Assimp 3.0"))
		{
			ShellExecuteA(NULL, "open", "http://assimp.sourceforge.net/lib_html/", NULL, NULL, SW_SHOWNORMAL);
		}

	
	}
	if (ImGui::CollapsingHeader("License"))
	{
		if (ImGui::MenuItem("License Apache 2.0"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/blob/master/LICENSE", NULL, NULL, SW_SHOWNORMAL);
		}
	}


	ImGui::End();
}

void ModuleImGui::ShowCreateGeometryWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Create Geometry", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	ImGui::Text("Fill the following parameters:");
	if (ImGui::CollapsingHeader("Sphere"))
	{
		static float createSphereRadius = 0;
		static float createSpherePosX = 0, createSpherePosY = 0, createSpherePosZ = 0;
		ImGui::SliderFloat("Sphere Radius", &createSphereRadius, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Sphere X", &createSpherePosX, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Sphere Y", &createSpherePosY, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Sphere Z", &createSpherePosZ, -5.0f, 5.0f, "%.2f");
		if (ImGui::Button("Create Sphere!") && createSphereRadius > 0.0f)
		{
			App->sceneEditor->AddSphere(createSphereRadius, vec3(createSpherePosX, createSpherePosY, createSpherePosZ));
			createSphereRadius = 0;
			createSpherePosX = 0;
			createSpherePosY = 0;
			createSpherePosZ = 0;
		}
	}
	if (ImGui::CollapsingHeader("Cube"))
	{
		static float createCubeX = 0, createCubeY = 0, createCubeZ = 0;
		static float createCubePosX = 0, createCubePosY = 0, createCubePosZ = 0;
		ImGui::SliderFloat("Cube X", &createCubeX, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube Y", &createCubeY, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube Z", &createCubeZ, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube Position X", &createCubePosX, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube Position Y", &createCubePosY, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube Position Z", &createCubePosZ, -5.0f, 5.0f, "%.2f");
		if (ImGui::Button("Create Cube!") && createCubeX > 0.0f && createCubeY > 0.0f && createCubeZ > 0.0f)
		{
			App->sceneEditor->AddCube(vec3(createCubeX, createCubeY, createCubeZ), vec3(createCubePosX, createCubePosY, createCubePosZ));
			createCubePosX = 0;
			createCubePosY = 0;
			createCubePosZ = 0;
			createCubeX = 0;
			createCubeY = 0;
			createCubeZ = 0;
		}
	}
	if (ImGui::CollapsingHeader("Cube drawn with vertex array"))
	{
		static float createCube1X = 0, createCube1Y = 0, createCube1Z = 0;
		static float createCube1PosX = 0, createCube1PosY = 0, createCube1PosZ = 0;
		ImGui::SliderFloat("Cube1 X", &createCube1X, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube1 Y", &createCube1Y, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube1 Z", &createCube1Z, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube1 Position X", &createCube1PosX, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube1 Position Y", &createCube1PosY, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube1 Position Z", &createCube1PosZ, -5.0f, 5.0f, "%.2f");
		if (ImGui::Button("Create Cube1!") && createCube1X > 0.0f && createCube1Y > 0.0f && createCube1Z > 0.0f)
		{
			App->sceneEditor->AddCube1(vec3(createCube1X, createCube1Y, createCube1Z), vec3(createCube1PosX, createCube1PosY, createCube1PosZ));
			createCube1PosX = 0;
			createCube1PosY = 0;
			createCube1PosZ = 0;
			createCube1X = 0;
			createCube1Y = 0;
			createCube1Z = 0;
		}
	}
	if (ImGui::CollapsingHeader("Cube drawn with indices (draw elements)"))
	{
		static float createCube2X = 0, createCube2Y = 0, createCube2Z = 0;
		static float createCube2PosX = 0, createCube2PosY = 0, createCube2PosZ = 0;
		ImGui::SliderFloat("Cube2 X", &createCube2X, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube2 Y", &createCube2Y, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube2 Z", &createCube2Z, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube2 Position X", &createCube2PosX, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube2 Position Y", &createCube2PosY, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Cube2 Position Z", &createCube2PosZ, -5.0f, 5.0f, "%.2f");
		if (ImGui::Button("Create Cube2!") && createCube2X > 0.0f && createCube2Y > 0.0f && createCube2Z > 0.0f)
		{
			App->sceneEditor->AddCube2(vec3(createCube2X, createCube2Y, createCube2Z), vec3(createCube2PosX, createCube2PosY, createCube2PosZ));
			createCube2PosX = 0;
			createCube2PosY = 0;
			createCube2PosZ = 0;
			createCube2X = 0;
			createCube2Y = 0;
			createCube2Z = 0;
		}
	}
	if (ImGui::CollapsingHeader("Cylinder"))
	{
		static float createCylinderRadius = 0, createCylinderHeight = 0;
		static float createCylinderPosX = 0, createCylinderPosY = 0, createCylinderPosZ = 0;
		ImGui::SliderFloat("Cylinder radius", &createCylinderRadius, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cylinder height", &createCylinderHeight, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Cylinder Position X", &createCylinderPosX, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Cylinder Position Y", &createCylinderPosY, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Cylinder Position Z", &createCylinderPosZ, -5.0f, 5.0f, "%.2f");
		if (ImGui::Button("Create Cylinder!") && createCylinderHeight > 0.0f && createCylinderRadius > 0.0f)
		{
			App->sceneEditor->AddCylinder(createCylinderRadius, createCylinderHeight, vec3(createCylinderPosX, createCylinderPosY, createCylinderPosZ));
			createCylinderRadius = 0;
			createCylinderHeight = 0;
			createCylinderPosX = 0;
			createCylinderPosY = 0;
			createCylinderPosZ = 0;
		}
	}
	if (ImGui::CollapsingHeader("Plane"))
	{
		//TODO: Find a purpose for a Plane Constant or remove it
		static float createPlaneX = 0, createPlaneY = 0, createPlaneZ = 0, createPlaneD = 0;
		static float createPlanePosX = 0, createPlanePosY = 0, createPlanePosZ = 0;
		ImGui::SliderFloat("Plane X", &createPlaneX, 0, 50.0f, "%.2f");
		ImGui::SliderFloat("Plane Y", &createPlaneY, 0, 50.0f, "%.2f");
		ImGui::SliderFloat("Plane Z", &createPlaneZ, 0, 50.0f, "%.2f");
	  //ImGui::SliderFloat("Plane Constant", &createPlaneD, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Plane Position X", &createPlanePosX, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Plane Position Y", &createPlanePosY, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Plane Position Z", &createPlanePosZ, -5.0f, 5.0f, "%.2f");
		if (ImGui::Button("Create Plane!") && createPlaneX > 0.0f && createPlaneY > 0.0f && createPlaneZ > 0.0f)
		{
			App->sceneEditor->AddPlaneNoGrid(createPlaneX, createPlaneY, createPlaneZ, createPlaneD, vec3(createPlanePosX, createPlanePosY, createPlanePosZ));
			createPlaneX = 0;
			createPlaneY = 0;
			createPlaneZ = 0;
			createPlaneD = 0;
			createPlanePosX = 0;
			createPlanePosY = 0;
			createPlanePosZ = 0;
		}
	}
	if (ImGui::CollapsingHeader("Capsule"))
	{
		static float createCapsuleRadius = 0, createCapsuleHeight = 0;
		static float createCapsulePosX = 0, createCapsulePosY = 0, createCapsulePosZ = 0;
		ImGui::SliderFloat("Capsule radius", &createCapsuleRadius, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Capsule height", &createCapsuleHeight, 0, 5.0f, "%.2f");
		ImGui::SliderFloat("Capsule Position X", &createCapsulePosX, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Capsule Position Y", &createCapsulePosY, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("Capsule Position Z", &createCapsulePosZ, -5.0f, 5.0f, "%.2f");
		if (ImGui::Button("Create Capsule!") && createCapsuleHeight > 0.0f && createCapsuleRadius > 0.0f)
		{
			App->sceneEditor->AddCylinder(createCapsuleRadius, createCapsuleHeight, vec3(createCapsulePosX, createCapsulePosY, createCapsulePosZ));
			App->sceneEditor->AddSphere(createCapsuleRadius, vec3(createCapsulePosX + createCapsuleHeight/2.0f, createCapsulePosY, createCapsulePosZ));
			App->sceneEditor->AddSphere(createCapsuleRadius, vec3(createCapsulePosX - createCapsuleHeight / 2.0f, createCapsulePosY, createCapsulePosZ));
			createCapsuleRadius = 0;
			createCapsuleHeight = 0;
			createCapsulePosX = 0;
			createCapsulePosY = 0;
			createCapsulePosZ = 0;
		}
	}

	ImGui::End();
}

void ModuleImGui::ShowEditorWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Show Editor", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	if (ImGui::CollapsingHeader("GameObjects Hieriarchy"))
	{
		App->sceneEditor->ShowEditor();
	}


	ImGui::End();
}