#pragma once

#include "Importer.h"

class ComponentMesh;

class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	void Load(const char* inputFile, ComponentMesh* mesh);
	bool Save(const ComponentMesh& mesh, const char* outputFile);
};