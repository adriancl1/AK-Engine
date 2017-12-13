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
		DrawBasicEditor();
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
