#include "MeshImporter.h"
#include "Application.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ModuleFileSystem.h"

#include "Glew\include\glew.h"
#include "Assimp\include\cimport.h" 
#include "Assimp\include\scene.h" 
#include "Assimp\include\postprocess.h" 
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

MeshImporter::MeshImporter()
{
}

MeshImporter::~MeshImporter()
{
}

void MeshImporter::Load(const char * inputFile, ResourceMesh* mesh)
{
	char* buffer;
	uint size;

	if (App->fileSystem->LoadFile(inputFile, &buffer, size, fileMesh) == true)
	{
		char* cursor = buffer;
		// amount of indices / vertices / colors / normals / texture_coords
		uint ranges[4];
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		mesh->numIndices = ranges[0];
		mesh->numVertices = ranges[1];

		// Load indices
		cursor += bytes;
		bytes = sizeof(uint) * mesh->numIndices;
		mesh->indices = new uint[mesh->numIndices];
		memcpy(mesh->indices, cursor, bytes);

		glGenBuffers(1, (GLuint*)&mesh->idIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->numIndices, mesh->indices, GL_STATIC_DRAW);

		// Load vertices
		cursor += bytes;
		bytes = sizeof(float) * mesh->numVertices * 3;
		mesh->vertices = new float[mesh->numVertices * 3];
		memcpy(mesh->vertices, cursor, bytes);

		glGenBuffers(1, (GLuint*)&mesh->idVertices);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->idVertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->numVertices * 3, mesh->vertices, GL_STATIC_DRAW);

		// Load Normals
		cursor += bytes;
		bytes = sizeof(float) * mesh->numVertices * 3;
		mesh->normals = new float[mesh->numVertices * 3];
		memcpy(mesh->normals, cursor, bytes);

		glGenBuffers(1, (GLuint*) &(mesh->idNormals));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->idNormals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->numVertices * 3, mesh->normals, GL_STATIC_DRAW);

		// Load TexCoords
		cursor += bytes;
		bytes = sizeof(float) * mesh->numVertices * 3;
		mesh->texCoords = new float[mesh->numVertices * 3];
		memcpy(mesh->texCoords, cursor, bytes);

		glGenBuffers(1, (GLuint*) &(mesh->idTexCoords));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->idTexCoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->numVertices * 3, mesh->texCoords, GL_STATIC_DRAW);

		mesh->enclosingBox.SetNegativeInfinity();

		mesh->enclosingBox.Enclose((float3*)mesh->vertices, mesh->numVertices);

		RELEASE_ARRAY(buffer);
	}
}

bool MeshImporter::Save(const aiMesh* mesh, const char * outputFile)
{
	ResourceMesh* tmpMesh = new ResourceMesh(0);

	tmpMesh->numVertices = mesh->mNumVertices;
	tmpMesh->vertices = new float[tmpMesh->numVertices * 3];
	memcpy(tmpMesh->vertices, mesh->mVertices, sizeof(float) * tmpMesh->numVertices * 3);
	LOG("Saving mesh with %d vertices", tmpMesh->numVertices);

	if (mesh->HasFaces())
	{
		tmpMesh->numIndices = mesh->mNumFaces * 3;
		tmpMesh->indices = new uint[tmpMesh->numIndices];
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&tmpMesh->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}
	if (mesh->HasNormals())
	{
		tmpMesh->normals = new float[tmpMesh->numVertices * 3];
		memcpy(tmpMesh->normals, mesh->mNormals, sizeof(float) * tmpMesh->numVertices * 3);
	}
	if (mesh->HasTextureCoords(0))
	{
		tmpMesh->texCoords = new float[tmpMesh->numVertices * 3];
		memcpy(tmpMesh->texCoords, mesh->mTextureCoords[0], sizeof(float) * tmpMesh->numVertices * 3);
	}

	uint ranges[4] = { tmpMesh->numIndices, tmpMesh->numVertices, tmpMesh->numVertices, tmpMesh->numVertices };

	float size = sizeof(ranges);
	size += sizeof(uint) * tmpMesh->numIndices; //Indices
	size += sizeof(float) * tmpMesh->numVertices * 3; //Vertices

	if (tmpMesh->normals != nullptr)
	{
		size += sizeof(float) * tmpMesh->numVertices * 3; //Normals
	}
	if (tmpMesh->texCoords != nullptr)
	{
		size += sizeof(float) * tmpMesh->numVertices * 3; //TexCoords
	}

	char* data = new char[size];
	char* cursor = data;

	uint bytes = 0;
	bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * tmpMesh->numIndices;
	memcpy(cursor, tmpMesh->indices, tmpMesh->numIndices * sizeof(uint));
	cursor += bytes;

	// Store vertices
	bytes = sizeof(float) * tmpMesh->numVertices * 3;
	memcpy(cursor, tmpMesh->vertices, tmpMesh->numVertices * 3 * sizeof(float));
	cursor += bytes;

	if (tmpMesh->normals != nullptr)
	{
		bytes = sizeof(float) * tmpMesh->numVertices * 3;
		memcpy(cursor, tmpMesh->normals, tmpMesh->numVertices * 3 * sizeof(float));
		cursor += bytes;
	}
	if (tmpMesh->texCoords != nullptr)
	{
		bytes = sizeof(float) * tmpMesh->numVertices * 3;
		memcpy(cursor, tmpMesh->texCoords, tmpMesh->numVertices * 3 * sizeof(float));
		cursor += bytes;
	}

	App->fileSystem->SaveFile(outputFile, data, size, fileMesh);

	delete tmpMesh;
	RELEASE_ARRAY(data);

	return true;
}
