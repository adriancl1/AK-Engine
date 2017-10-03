#include "ModuleGeometryImporter.h"
#include "Application.h"
#include "ModuleSceneEditor.h"

#include "Assimp\include\cimport.h" 
#include "Assimp\include\scene.h" 
#include "Assimp\include\postprocess.h" 
#include "Assimp\include\cfileio.h"
#include "Glew\include\glew.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleGeometryImporter::ModuleGeometryImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Geometry Importer";
}

ModuleGeometryImporter::~ModuleGeometryImporter()
{
}

bool ModuleGeometryImporter::Init(JSON_Object* data)
{
	struct aiLogStream stream; 
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr); 
	aiAttachLogStream(&stream);
	return true;
}

bool ModuleGeometryImporter::LoadMesh(const char* fullPath)
{
	const aiScene* scene = aiImportFile(fullPath, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes()) 
	{ 
		LOG("Scene %s loaded succesfully", fullPath); 
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* newMesh = scene->mMeshes[i];
			Mesh* m = new Mesh;
			m->numVertices = newMesh->mNumVertices;
			m->vertices = new float[m->numVertices * 3];
			memcpy(m->vertices, newMesh->mVertices, sizeof(float)* m->numVertices * 3);
			LOG("New mesh with %d vertices", m->numVertices);
			if (newMesh->HasFaces())
			{
				m->numIndices = newMesh->mNumFaces * 3;
				m->indices = new GLushort[m->numIndices];
				for (uint i = 0; i < newMesh->mNumFaces; ++i) 
				{ 
					if (newMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m->indices[i * 3], newMesh->mFaces[i].mIndices, 3 * sizeof(GLushort));
					}
				}
				glGenBuffers(1, &m->idVertices);
				glBindBuffer(GL_ARRAY_BUFFER, m->idVertices);
				glBufferData(GL_ARRAY_BUFFER, sizeof(m->vertices), m->vertices, GL_STATIC_DRAW); 

				glGenBuffers(1, &m->idIndices);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->idIndices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m->indices), m->indices, GL_STATIC_DRAW);
			}
			else
			{
				glGenBuffers(1, &m->idVertices);
				glBindBuffer(GL_ARRAY_BUFFER, m->idVertices);
				glBufferData(GL_ARRAY_BUFFER, sizeof(m->vertices), m->vertices, GL_STATIC_DRAW);
			}
			App->sceneEditor->AddMesh(m);
		}
		aiReleaseImport(scene); 
		return true;
	} 
	else {
		LOG("Error loading scene %s", fullPath);
		return false;
	}
}

bool ModuleGeometryImporter::CleanUp(JSON_Object* data)
{
	return true;
}

