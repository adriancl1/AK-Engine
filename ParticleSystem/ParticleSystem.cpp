#include "ParticleSystem.h"
#include "Emiter.h"
#include "../imgui-1.51/imgui.h"
#include "GlobalDefines.h"
ParticleSystem::ParticleSystem()
{
	emiter = new Emiter();


}

ParticleSystem::~ParticleSystem()
{
	RELEASE(emiter);
}

void ParticleSystem::DrawParticleSystemEditor()
{
	if (!windowShow)
		return;

	if (ImGui::CollapsingHeader("Basic"))
	{

	}

	if (ImGui::CollapsingHeader("Shapes"))
	{
	emiter->DrawEmiterEditor();
	}
	if (ImGui::CollapsingHeader("Renderer"))
	{

	}
}

void ParticleSystem::Draw()
{
	emiter->DrawEmiter();

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
