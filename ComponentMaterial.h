#pragma once

#include "Component.h"
#include "Globals.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial() : Component(Component_Material)
	{}

	uint idTexture;
	

};