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
	/*this could also be done by simply saving the vector directly without being variable to variable*/

	std::vector<float> vectEmiter = particleSystem->SaveEmiter();;
	

	data.SetFloat("e_emiterTime", vectEmiter[0]);
	data.SetFloat("e_loop", vectEmiter[1]);
	data.SetFloat("e_particleRate", vectEmiter[2]);
	data.SetFloat("e_timePLife", vectEmiter[3]);

	data.SetFloat("e_modTimePlife", vectEmiter[4]);
	data.SetFloat("e_speed", vectEmiter[5]);
	data.SetFloat("e_modSpeed", vectEmiter[6]);
	data.SetFloat("e_emiterTime", vectEmiter[7]);


	std::vector<float> initPart = particleSystem->SaveInitialState();;


	data.SetFloat("i_size1", initPart[0]);
	data.SetFloat("i_size2", initPart[1]);
	data.SetFloat("i_gravity", initPart[2]);
	data.SetFloat("i_gravityVariation", initPart[3]);

	data.SetFloat("i_rotation", initPart[4]);
	data.SetFloat("i_rotation2", initPart[5]);
	data.SetFloat("i_speed", initPart[6]);

	data.SetFloat("i_color_x", initPart[7]);
	data.SetFloat("i_color_y", initPart[8]);
	data.SetFloat("i_color_z", initPart[9]);
	data.SetFloat("i_color_w", initPart[10]);

	data.SetFloat("i_color2_x", initPart[11]);
	data.SetFloat("i_color2_x", initPart[12]);
	data.SetFloat("i_color2_z", initPart[13]);
	data.SetFloat("i_color2_w", initPart[14]);
	
	std::vector<float> finalPart = particleSystem->SaveEndState();;

	
	data.SetFloat("f_size1", finalPart[0]);
	data.SetFloat("f_size2", finalPart[1]);
	data.SetFloat("f_gravity", finalPart[2]);
	data.SetFloat("f_gravityVariation", finalPart[3]);

	data.SetFloat("f_rotation", finalPart[4]);
	data.SetFloat("f_rotation2", finalPart[5]);
	data.SetFloat("f_speed", finalPart[6]);

	data.SetFloat("f_color_x", finalPart[7]);
	data.SetFloat("f_color_y", finalPart[8]);
	data.SetFloat("f_color_z", finalPart[9]);
	data.SetFloat("f_color_w", finalPart[10]);

	data.SetFloat("f_color2_x", finalPart[11]);
	data.SetFloat("f_color2_x", finalPart[12]);
	data.SetFloat("f_color2_z", finalPart[13]);
	data.SetFloat("f_color2_w", finalPart[14]);


}

void ComponentParticles::OnLoad(Configuration & data)
{
	/*this could also be done by simply loading the vector directly without being variable to variable*/
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
