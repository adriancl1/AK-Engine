#pragma once

#include "Importer.h"

class ResourceAnimation;
class aiAnimation;

class AnimationImporter : public Importer
{
public:
	AnimationImporter();
	~AnimationImporter();

	void Load(const char* inputFile, ResourceAnimation* anim);
	bool Save(const aiAnimation* anim, const char* outputFile);
};