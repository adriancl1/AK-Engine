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

			//VERTICES
			m->numVertices = newMesh->mNumVertices;
			m->vertices = new float[m->numVertices * 3];
			memcpy(m->vertices, newMesh->mVertices, sizeof(float)* m->numVertices * 3);
			LOG("New mesh with %d vertices", m->numVertices);

			glGenBuffers(1,(GLuint*) &m->idVertices);
			glBindBuffer(GL_ARRAY_BUFFER, m->idVertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->numVertices * 3, m->vertices, GL_STATIC_DRAW);

			//INDICES
			if (newMesh->HasFaces())
			{
				m->numIndices = newMesh->mNumFaces * 3;
				m->indices = new uint[m->numIndices];
				for (uint i = 0; i < newMesh->mNumFaces; ++i) 
				{ 
					if (newMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m->indices[i * 3], newMesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				} 

				glGenBuffers(1,(GLuint*) &m->idIndices);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->idIndices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m->numIndices, m->indices, GL_STATIC_DRAW);
			}

			//NORMALS
			if (newMesh->HasNormals())
			{
				m->normals = new float[m->numVertices * 3];
				memcpy(m->normals, newMesh->mNormals, sizeof(float) * m->numVertices * 3);

				glGenBuffers(1, (GLuint*) &(m->idNormals));
				glBindBuffer(GL_ARRAY_BUFFER, m->idNormals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->numVertices * 3, m->normals, GL_STATIC_DRAW);
			}

			//COLORS
			if (newMesh->HasVertexColors(0))
			{
				m->colors = new float[m->numVertices * 3];
				memcpy(m->colors, newMesh->mColors, sizeof(float) * m->numVertices * 3);

				glGenBuffers(1, (GLuint*) &(m->idColors));
				glBindBuffer(GL_ARRAY_BUFFER, m->idColors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->numVertices * 3, m->colors, GL_STATIC_DRAW);
			}

			//TEXTURE COORDS
			if (newMesh->HasTextureCoords(0))
			{
				m->texCoords = new float[m->numVertices * 3];
				memcpy(m->texCoords, newMesh->mTextureCoords[0], sizeof(float) * m->numVertices * 3);

				glGenBuffers(1, (GLuint*) &(m->idTexCoords));
				glBindBuffer(GL_ARRAY_BUFFER, m->idTexCoords);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->numVertices * 3, m->texCoords, GL_STATIC_DRAW);
			}

			m->enclosingBox.SetNegativeInfinity();
			m->enclosingBox.Enclose((float3*)m->vertices, m->numVertices);

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

