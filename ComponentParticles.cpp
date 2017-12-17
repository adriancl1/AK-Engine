#include "ComponentParticles.h"
#include "Configuration.h"
#include "imgui-1.51/imgui.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Application.h"
#include <vector>

ComponentParticles::ComponentParticles() : Component(Component_Particles)
{
	name = "Particle System";
	
	particleSystem = new ParticleSystem();
	
}

ComponentParticles::~ComponentParticles()
{
	RELEASE(particleSystem);
}

void ComponentParticles::PreUpdate(float dt)
{
	//cuando se le de al Play
	
}

void ComponentParticles::Update(float dt)
{

	ComponentCamera* camera = App->camera->GetEditorCamera();
	particleSystem->cameraPos = camera->GetFrustum().pos;

	particleSystem->Update(dt);

	
	ComponentTransform* myTransform = (ComponentTransform*)myGO->FindComponent(Component_Transform);
	
	particleSystem->SetTransform(myTransform->GetPosition(), myTransform->GetRotation(), {1,1,1});
	particleSystem->Draw();
}

void ComponentParticles::OnPlay()
{
	particleSystem->Play();
}

void ComponentParticles::OnPause()
{
	particleSystem->Pause();
}

void ComponentParticles::OnEditor()
{

	if (ImGui::TreeNodeEx(name.c_str()))
	{
		particleSystem->DrawParticleSystemEditor();

		ImGui::TreePop();
	}
}

void ComponentParticles::OnSave(Configuration & data) const
{
	std::vector<float>* vectEmiter[8];
	std::vector<float>* initPart[15];
	std::vector<float>* finalPart[15];

//	particleSystem->Save(*vectEmiter[8], *initPart[15], *finalPart[15]);




}

void ComponentParticles::OnLoad(Configuration & data)
{
	float temp;
	std::vector<float> vectEmiter[8];

	vectEmiter->push_back(data.GetFloat("e_emiterTime"));
	vectEmiter->push_back(data.GetFloat("e_loop"));
	vectEmiter->push_back(data.GetFloat("e_particleRate"));
	vectEmiter->push_back(data.GetFloat("e_timePLife"));

	vectEmiter->push_back(data.GetFloat("e_modTimePlife"));
	vectEmiter->push_back(data.GetFloat("e_speed"));
	vectEmiter->push_back(data.GetFloat("e_modSpeed"));
	vectEmiter->push_back(data.GetFloat("e_emiterTime"));

	std::vector<float> initPart[15];

	initPart->push_back(data.GetFloat("i_size1"));
	initPart->push_back(data.GetFloat("i_size2"));
	initPart->push_back(data.GetFloat("i_gravity"));
	initPart->push_back(data.GetFloat("i_gravityVariation"));
	initPart->push_back(data.GetFloat("i_rotation"));
	initPart->push_back(data.GetFloat("i_rotation2"));
	initPart->push_back(data.GetFloat("i_speed"));

	initPart->push_back(data.GetFloat("i_color_x"));
	initPart->push_back(data.GetFloat("i_color_y"));
	initPart->push_back(data.GetFloat("i_color_z"));
	initPart->push_back(data.GetFloat("i_color_w"));

	initPart->push_back(data.GetFloat("i_color2_x"));
	initPart->push_back(data.GetFloat("i_color2_y"));
	initPart->push_back(data.GetFloat("i_color2_z"));
	initPart->push_back(data.GetFloat("i_color2_w"));


	std::vector<float> finalPart[15];

	finalPart->push_back(data.GetFloat("f_size1"));
	finalPart->push_back(data.GetFloat("f_size2"));
	finalPart->push_back(data.GetFloat("f_gravity"));
	finalPart->push_back(data.GetFloat("f_gravityVariation"));
	finalPart->push_back(data.GetFloat("f_rotation"));
	finalPart->push_back(data.GetFloat("f_rotation2"));
	finalPart->push_back(data.GetFloat("f_speed"));

	finalPart->push_back(data.GetFloat("f_color_x"));
	finalPart->push_back(data.GetFloat("f_color_y"));
	finalPart->push_back(data.GetFloat("f_color_z"));
	finalPart->push_back(data.GetFloat("f_color_w"));

	finalPart->push_back(data.GetFloat("f_color2_x"));
	finalPart->push_back(data.GetFloat("f_color2_y"));
	finalPart->push_back(data.GetFloat("f_color2_z"));
	finalPart->push_back(data.GetFloat("f_color2_w"));

}
