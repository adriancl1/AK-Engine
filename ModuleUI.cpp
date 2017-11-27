#include "ModuleUI.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled): Module(app,start_enabled) 
{
	name = "UI";
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Start()
{
	return true;
}

void ModuleUI::ReceiveInput()
{

}

bool ModuleUI::CleanUp()
{
	return true;
}
