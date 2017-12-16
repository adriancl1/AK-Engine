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

	if (ps_state != PS_PLAYING || emiter->data.loop != true /*&& emiter->data.emiterTime > emiter->data.timeToEmite*/)
		return ret;

	this->ps_dt += dt;

	//if (ps_dt <= emiter->data.particleRate)
	//{

		uint nParticles = ps_dt / emiter->data.particleRate;

		for (int i = 0; i < nParticles; i++)
			CreateParticle();

		ps_dt -= emiter->data.particleRate * nParticles;


//	}

	for (std::vector<Particle*>::iterator it = particleVec.begin(); it != particleVec.end(); ++it)
	{
		ret = (*it)->Update(dt);
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

void ParticleSystem::DrawParticleSystemEditor()
{
	if (!windowShow)
		return;

	ImGui::Text("Number of particles %i",particleVec.size());


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
			ImGui::SliderFloat("Gravity", (float*)&initialState.gravity, -50, 50);
			ImGui::SliderFloat("Size", (float*)&initialState.size, 0, 1000);

			if (ImGui::TreeNodeEx("Color"))
			{
				float col2[4] = { initialState.color.x,initialState.color.y,initialState.color.z,initialState.color.w };

				ImGui::ColorPicker4("Color", col2);

				initialState.color.x = col2[0];
				initialState.color.y = col2[1];
				initialState.color.z = col2[2];
				initialState.color.w = col2[3];
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Color2"))
			{
				float col2[4] = { initialState.color.x,initialState.color.y,initialState.color.z,initialState.color.w };

				ImGui::ColorPicker4("Color", col2);

				initialState.color2.x = col2[0];
				initialState.color2.y = col2[1];
				initialState.color2.z = col2[2];
				initialState.color2.w = col2[3];
				ImGui::TreePop();
			}


		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Final"))
	{
			ImGui::SliderFloat("Speed", (float*)&finalState.speed, 0, 50);
			ImGui::SliderFloat("Gravity", (float*)&finalState.gravity, -50, 50);
			ImGui::SliderFloat("Size", (float*)&finalState.size, 0, 1000);

			if (ImGui::TreeNodeEx("Color"))
			{
				float col2[4] = { finalState.color.x,finalState.color.y,finalState.color.z,finalState.color.w };

				ImGui::ColorPicker4("Color", col2);

				finalState.color.x = col2[0];
				finalState.color.y = col2[1];
				finalState.color.z = col2[2];
				finalState.color.w = col2[3];
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
			}

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

	float vertex[] = {-0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.5f, -0.5f, 0.f, -0.5f, -0.5f, 0.f};

	particleMesh->vertices = new float[particleMesh->numVertices * 3];
	memcpy(particleMesh->vertices, vertex, sizeof(float) * particleMesh->numVertices * 3);

	glGenBuffers(1, (GLuint*)&particleMesh->idVertices);
	glBindBuffer(GL_ARRAY_BUFFER, particleMesh->idVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * particleMesh->numVertices * 3, particleMesh->vertices, GL_STATIC_DRAW);

	particleMesh->numIndices = 6;
	uint indices[] = {2,1,0,2,3,1};
	particleMesh->indices = new uint[6];
	memcpy(particleMesh->indices, indices, sizeof(uint)*particleMesh->numVertices);
	
	glGenBuffers(1, (GLuint*)&particleMesh->idIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleMesh->idIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * particleMesh->idIndices, particleMesh->indices, GL_STATIC_DRAW);

	float colors[]={ 0, 1.f, 0,1.f, 1.f, 0,	0, 0, 0,1.f, 0, 0 };
	particleMesh->colors = new float[particleMesh->numVertices * 3];
	memcpy(particleMesh->colors, colors, sizeof(float) * particleMesh->numVertices * 3);

	glGenBuffers(1, (GLuint*)&particleMesh->idColors);
	glBindBuffer(GL_ARRAY_BUFFER, particleMesh->idColors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * particleMesh->numIndices * 3, particleMesh->colors, GL_STATIC_DRAW);
	
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


