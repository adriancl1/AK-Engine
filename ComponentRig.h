#pragma once

#include "Globals.h"
#include "Component.h"

class ResourceRig;

class ComponentRig : public Component
{
public:
	ComponentRig();
	~ComponentRig();

	void OnEditor()override;

	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;
	void AddResource(int UID) override;

private:
	ResourceRig* rig;
};