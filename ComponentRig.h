#pragma once

#include "Globals.h"
#include "Component.h"

#include <vector>

class ResourceRig;

class ComponentRig : public Component
{
public:
	ComponentRig();
	~ComponentRig();

	void Update()override;
	void PostUpdate()override;

	void DrawDebug()const;

	void OnEditor()override;

	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;
	void AddResource(int UID) override;

private:
	ResourceRig* rig;
	bool assignedBones = false;
	std::vector <GameObject*> bonesGO;
	bool debug = false;
};