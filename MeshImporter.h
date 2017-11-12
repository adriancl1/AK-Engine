#pragma once

#include "Importer.h"

class ResourceMesh;
class aiMesh;

class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	void Load(const char* inputFile, ResourceMesh* mesh);
	bool Save(const aiMesh* mesh, const char* outputFile);
};