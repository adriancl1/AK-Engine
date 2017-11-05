#include "MeshImporter.h"
#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleFileSystem.h"

#include "Glew\include\glew.h"

MeshImporter::MeshImporter()
{
}

MeshImporter::~MeshImporter()
{
}

void MeshImporter::Load(const char * inputFile, ComponentMesh* mesh)
{
	char* buffer;
	uint size;

	if (App->fileSystem->LoadFile(inputFile, &buffer, size, fileMesh) == true)
	{
		char* cursor = buffer;
		// amount of indices / vertices / colors / normals / texture_coords
		uint ranges[2];
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
	}
}

bool MeshImporter::Save(const ComponentMesh & mesh, const char * outputFile)
{
	uint ranges[2] = { mesh.numIndices, mesh.numVertices };
	float size = sizeof(ranges);
	size += sizeof(uint) * mesh.numIndices;
	size += sizeof(float) * mesh.numVertices * 3;

	char* data = new char[size];
	char* cursor = data;

	uint bytes = 0;
	bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * mesh.numIndices;
	memcpy(cursor, mesh.indices, mesh.numIndices * sizeof(uint));
	cursor += bytes;

	// Store vertices
	bytes = sizeof(float) * mesh.numVertices * 3;
	memcpy(cursor, mesh.vertices, mesh.numVertices * 3 * sizeof(float));

	App->fileSystem->SaveFile(outputFile, data, size, fileMesh);

	RELEASE_ARRAY(data);

	return true;
}
