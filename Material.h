#pragma once

#include "Component.h"
#include "Globals.h"

class ComponentMaterial : public Component
{
	ComponentMaterial() : Component(Component_Material)
	{}

	uint idTexture;
};