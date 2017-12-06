#pragma once

#include "Importer.h"

class ResourceRig;
class aiMesh;

class RigImporter : public Importer
{
public:
	RigImporter();
	~RigImporter();

	void Load(const char* inputFile, ResourceRig* rig);
	bool Save(const aiMesh* mesh, const char* outputFile);
};