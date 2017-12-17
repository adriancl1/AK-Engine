#include "ParticleSystem.h"
#include "Emiter.h"
#include "../imgui-1.51/imgui.h"
#include "GlobalDefines.h"
#include "Particle.h"


ParticleSystem::ParticleSystem()
{

	transformation = new PTransformation();

	particleMesh = new ParticleMesh();
	SetPlaneMesh();

	emiter = new Emiter();
	emiter->pSystem = this;
}

ParticleSystem::~ParticleSystem()
{
	RELEASE(emiter);
	RELEASE(transformation);
	RELEASE(particleMesh);



	//for (std::vector<Particle*>::iterator it = particleVec.end(); it != particleVec.begin(); --it)
	//{
	//	particleVec.pop_back();
	//}
}

bool ParticleSystem::PreUpdate(float dt)
{
	
	emiter->data.emiterTime = dt; // set the time of play to the editr timme
	return true;
}

bool ParticleSystem::Update(float dt)
{
	bool ret = true;
	
	//tData.textureID = App->textures->ImportImage("../Game/Assets/Baker_house.png");
	if (ps_state != PS_PLAYING  /*&& emiter->data.emiterTime > emiter->data.timeToEmite*/)
		return ret;

	this->control += dt;

	this->ps_dt += dt;

	if (control <= emiter->data.timeToEmite || emiter->data.loop == true)
	{

		uint nParticles = ps_dt / emiter->data.particleRate;
		for (int i = 0; i < nParticles; i++)
		{
			CreateParticle();
			size++;
		}

		ps_dt -= emiter->data.particleRate * nParticles;
	}

	for (std::vector<Particle*>::iterator it = particleVec.begin(); it != particleVec.end(); ++it)
	{
	
		ret = (*it)->Update(dt);
		if ((*it)->KillParticle() == true)
		{			
			size--;
		}
	}


	return ret;
}

bool ParticleSystem::PostUpdate(float dt)
{
	return false;
}

void ParticleSystem::Stop()
{
	ps_state = PS_STOP;

	for (std::vector<Particle*>::iterator it = particleVec.begin(); it != particleVec.end(); ++it)
		(*it)->KillParticle();


}

void ParticleSystem::Play()
{
	ps_state = PS_PLAYING;
}

void ParticleSystem::Pause()
{
	ps_state = PS_PAUSE;
}

void ParticleSystem::Load(std::vector<float> vectEmiter, std::vector<float>initPart, std::vector<float>finalPart)
{
	// EMITER
	emiter->data.emiterTime = vectEmiter[0];

	float booll = vectEmiter[1];

	if (booll > 0.5) 	
		emiter->data.loop = true;
	else 
		emiter->data.loop = false;

	emiter->data.particleRate = vectEmiter[2];
	emiter->data.timePLife = vectEmiter[3];
	emiter->data.modTimePlife = vectEmiter[4];
	emiter->data.speed = vectEmiter[5];
	emiter->data.modSpeed = vectEmiter[6];
	emiter->data.emiterTime = vectEmiter[7];

	//INIT

	this->initialState.size1 = initPart[0];
	this->initialState.size2 = initPart[1];

	this->initialState.gravity = initPart[2];
	this->initialState.gravityVariation = initPart[3];

	this->initialState.rotation = initPart[4];
	this->initialState.rotation2 = initPart[5];

	this->initialState.speed = initPart[6];

	this->initialState.color.x = initPart[7];
	this->initialState.color.y = initPart[8];
	this->initialState.color.z = initPart[9];
	this->initialState.color.w = initPart[10];

	this->initialState.color2.x = initPart[11];
	this->initialState.color2.y = initPart[12];
	this->initialState.color2.z = initPart[13];
	this->initialState.color2.w = initPart[14];


	//END
	this->finalState.size1 = finalPart[0];
	this->finalState.size2 = finalPart[1];

	this->finalState.gravity = finalPart[2];
	this->finalState.gravityVariation = finalPart[3];

	this->finalState.rotation = finalPart[4];
	this->finalState.rotation2 = finalPart[5];

	this->finalState.speed = finalPart[6];

	this->finalState.color.x = finalPart[7];
	this->finalState.color.y = finalPart[8];
	this->finalState.color.z = finalPart[9];
	this->finalState.color.w = finalPart[10];

	this->finalState.color2.x = finalPart[11];
	this->finalState.color2.y = finalPart[12];
	this->finalState.color2.z = finalPart[13];
	this->finalState.color2.w = finalPart[14];


}

void ParticleSystem::Save(std::vector<float>& vectEmiter, std::vector<float>& initPart, std::vector<float>& finalPart)
{


	//INIT--------------------------------------

	initPart[0] = this->initialState.size1;
	initPart[1] = this->initialState.size2;

	initPart[2] = this->initialState.gravity;
	initPart[3] = this->initialState.gravityVariation;

	initPart[4] = this->initialState.rotation;
	initPart[5] = this->initialState.rotation2;

	initPart[6] = this->initialState.speed;

	initPart[7] = this->initialState.color.x;
	initPart[8] = this->initialState.color.y;
	initPart[9] = this->initialState.color.z;
	initPart[10] = this->initialState.color.w;

	initPart[11] = this->initialState.color2.x;
	initPart[12] = this->initialState.color2.y;
	initPart[13] = this->initialState.color2.z;
	initPart[14] = this->initialState.color2.w;

	//SAVE--------------------------------------

	finalPart[0] = this->finalState.size1;
	finalPart[1] = this->finalState.size2;

	finalPart[2] = this->finalState.gravity;
	finalPart[3] = this->finalState.gravityVariation;

	finalPart[4] = this->finalState.rotation;
	finalPart[5] = this->finalState.rotation2;

	finalPart[6] = this->finalState.speed;

	finalPart[7] = this->finalState.color.x;
	finalPart[8] = this->finalState.color.y;
	finalPart[9] = this->finalState.color.z;
	finalPart[10] = this->finalState.color.w;

	finalPart[11] = this->finalState.color2.x;
	finalPart[12] = this->finalState.color2.y;
	finalPart[13] = this->finalState.color2.z;
	finalPart[14] = this->finalState.color2.w;

}

std::vector<float> ParticleSystem::SaveEmiter()
{

	std::vector<float>vectEmiter;
	
	//emiter ------------------------------------
	vectEmiter.push_back(emiter->data.emiterTime);

	if (emiter->data.loop == true)
		vectEmiter.push_back(1.f);
	else
		vectEmiter.push_back(0.f);

	vectEmiter.push_back(emiter->data.particleRate);
	vectEmiter.push_back(emiter->data.timePLife);
	vectEmiter.push_back(emiter->data.modTimePlife);
	vectEmiter.push_back(emiter->data.speed);
	vectEmiter.push_back(emiter->data.modSpeed);
	vectEmiter.push_back(emiter->data.emiterTime);
	
	return vectEmiter;
}

std::vector<float> ParticleSystem::SaveEndState()
{
	std::vector<float>finalPart;

	finalPart.push_back(this->finalState.size1);
	finalPart.push_back(this->finalState.size2);

	finalPart.push_back(this->finalState.gravity);
	finalPart.push_back(this->finalState.gravityVariation);

	finalPart.push_back(this->finalState.rotation);
	finalPart.push_back(this->finalState.rotation2);

	finalPart.push_back(this->finalState.speed);

	finalPart.push_back(this->finalState.color.x);
	finalPart.push_back(this->finalState.color.y);
	finalPart.push_back(this->finalState.color.z);
	finalPart.push_back(this->finalState.color.w);

	finalPart.push_back(this->finalState.color2.x);
	finalPart.push_back(this->finalState.color2.y);
	finalPart.push_back(this->finalState.color2.z);
	finalPart.push_back(this->finalState.color2.w);


	return std::vector<float>(finalPart);
}

std::vector<float> ParticleSystem::SaveInitialState()
{
	std::vector<float>initPart;

	initPart.push_back(this->initialState.size1);
	initPart.push_back(this->initialState.size2);

	initPart.push_back(this->initialState.gravity);
	initPart.push_back(this->initialState.gravityVariation);

	initPart.push_back(this->initialState.rotation);
	initPart.push_back(this->initialState.rotation2);

	initPart.push_back(this->initialState.speed);

	initPart.push_back(this->initialState.color.x);
	initPart.push_back(this->initialState.color.y);
	initPart.push_back(this->initialState.color.z);
	initPart.push_back(this->initialState.color.w);

	initPart.push_back(this->initialState.color2.x);
	initPart.push_back(this->initialState.color2.y);
	initPart.push_back(this->initialState.color2.z);
	initPart.push_back(this->initialState.color2.w);


	return std::vector<float>(initPart);
}

void ParticleSystem::DrawParticleSystemEditor()
{
	if (!windowShow)
		return;
	
	ImGui::Text("Number of particles %i", particleVec.size());


	if (ImGui::CollapsingHeader("Basic"))
	{
		DrawBasicEditor();
	}

	if (ImGui::CollapsingHeader("Emiter"))
	{
		emiter->DrawEmiterEditor();
	}

}

void ParticleSystem::Draw()
{
	//CreateParticle();
	emiter->DrawEmiter();

}

void ParticleSystem::DrawBasicEditor()
{
	if (ImGui::TreeNodeEx("Initial"))
	{
			ImGui::SliderFloat("Speed", (float*)&initialState.speed, 0, 50);
			ImGui::SliderFloat("Gravity", (float*)&initialState.gravity, -10, 10);
			ImGui::SliderFloat("Gravity Variation", (float*)&initialState.gravityVariation, -5, 5);
			ImGui::Separator();
			ImGui::SliderFloat("rot1", (float*)&initialState.rotation, 0, 10);
			ImGui::SliderFloat("rot2", (float*)&initialState.rotation2, 0, 10);
			ImGui::Separator();
			ImGui::SliderFloat("Size1", (float*)&initialState.size1, 0, 30);
			ImGui::SliderFloat("Size2", (float*)&initialState.size2, 0, 30);
			ImGui::Separator();

			if (ImGui::TreeNodeEx("Color"))
			{
				float col[4] = { initialState.color.x,initialState.color.y,initialState.color.z,initialState.color.w };

				ImGui::ColorPicker4("Color", col);

				initialState.color.x = col[0];
				initialState.color.y = col[1];
				initialState.color.z = col[2];
				initialState.color.w = col[3];
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Color2"))
			{
				float col3[4] = { initialState.color2.x,initialState.color2.y,initialState.color2.z,initialState.color2.w };

				ImGui::ColorPicker4("Color", col3);

				initialState.color2.x = col3[0];
				initialState.color2.y = col3[1];
				initialState.color2.z = col3[2];
				initialState.color2.w = col3[3];
				ImGui::TreePop();
			}
			ImGui::Separator();
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Final"))
	{
			ImGui::SliderFloat("Speed", (float*)&finalState.speed, 0, 50);
			ImGui::SliderFloat("Gravity", (float*)&finalState.gravity, -10, 10);
			ImGui::SliderFloat("Gravity Variation", (float*)&finalState.gravityVariation, -5, 5);
			ImGui::Separator();
			ImGui::SliderFloat("rot1", (float*)&finalState.rotation, 0, 10);
			ImGui::SliderFloat("rot2", (float*)&finalState.rotation2, 0, 10);
			ImGui::Separator();
			ImGui::Separator();
			ImGui::SliderFloat("Size1", (float*)&finalState.size1, 0, 100);
			ImGui::SliderFloat("Size2", (float*)&finalState.size2, 0, 100);
			ImGui::Separator();
			if (ImGui::TreeNodeEx("Color"))
			{
				float col1[4] = { finalState.color.x,finalState.color.y,finalState.color.z,finalState.color.w };

				ImGui::ColorPicker4("Color", col1);

				finalState.color.x = col1[0];
				finalState.color.y = col1[1];
				finalState.color.z = col1[2];
				finalState.color.w = col1[3];
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Color2"))
			{
				float col2[4] = { finalState.color2.x,finalState.color2.y,finalState.color2.z,finalState.color2.w };

				ImGui::ColorPicker4("Color", col2);

				finalState.color2.x = col2[0];
				finalState.color2.y = col2[1];
				finalState.color2.z = col2[2];
				finalState.color2.w = col2[3];

				ImGui::TreePop();
			}			ImGui::Separator();

		ImGui::TreePop();
	}


}

void ParticleSystem::openCloseWindowPS(bool state)
{
	windowShow = state;
}

bool ParticleSystem::isOpenCloseWindowPS()
{
	return windowShow;
}

void ParticleSystem::changeOpenCloseWindowPS()
{
	windowShow = !windowShow;
}

void ParticleSystem::SetTransform(float3 Position, Quat Rotation, float3 Scale)
{
	transformation->position = Position;
	transformation->rotation = Rotation;
	transformation->scale = Scale;

}

ParticleMesh* ParticleSystem::GetMesh() const
{
	return particleMesh;
}

void ParticleSystem::SetPlaneMesh()
{	
	particleMesh->numVertices = 4;
	particleMesh->numFaces = 2;

	float startSize = this->initialState.size1;

	float vertex[] = {-startSize, startSize, 0.f, startSize, startSize, 0.f, -startSize, -startSize, 0.f, startSize, -startSize, 0.f};

	particleMesh->vertices = new float[particleMesh->numVertices * 3];
	memcpy(particleMesh->vertices, vertex, sizeof(float) * particleMesh->numVertices * 3);

	glGenBuffers(1, (GLuint*)&particleMesh->idVertices);
	glBindBuffer(GL_ARRAY_BUFFER, particleMesh->idVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * particleMesh->numVertices * 3, particleMesh->vertices, GL_STATIC_DRAW);

	particleMesh->numIndices = 6;
	uint indices[] = {2,1,0,2,3,1};
	particleMesh->indices = new uint[6];
	memcpy(particleMesh->indices, indices, sizeof(float)*particleMesh->numIndices);
	
	glGenBuffers(1, (GLuint*)&particleMesh->idIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleMesh->idIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * particleMesh->idIndices, particleMesh->indices, GL_STATIC_DRAW);
	
	
	particleMesh->colors = new float[particleMesh->numVertices * 3];
	memcpy(particleMesh->colors, particleMesh->colors, sizeof(float) * particleMesh->numVertices * 3);

	glGenBuffers(1, (GLuint*)&particleMesh->idColors);
	glBindBuffer(GL_ARRAY_BUFFER, particleMesh->idColors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * particleMesh->numIndices * 6, particleMesh->colors, GL_STATIC_DRAW);
	
}

void ParticleSystem::SetActive()
{
	active = true;
}

void ParticleSystem::SetDesactive()
{
	active = false;
}

void ParticleSystem::CreateParticle()
{
	LCG rGen;
	float3 direction;

	switch (emiter->type) 
	{
		
	case E_SPHERE:		
		direction = emiter->shape.sphere.RandomPointOnSurface(rGen);
			break;	
	};

	Particle* nParticle = new Particle(this, initialState, finalState, direction, emiter->data.timePLife);
	particleVec.push_back(nParticle);
}


