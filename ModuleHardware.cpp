#include "ModuleHardware.h"
#include "Application.h"
#include "Glew\include\glew.h"


#pragma comment( lib, "Glew/libx86/glew32.lib" )

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

ModuleHardware::ModuleHardware(Application* app, bool starteEnabled) : Module(app, starteEnabled)
{
	name = "hardware";
}

ModuleHardware::~ModuleHardware()
{
}

void ModuleHardware::OnConfiguration()
{
	if (ImGui::CollapsingHeader("Hardware"))
	{
		SDL_version compiled;
		SDL_VERSION(&compiled);

		ImGui::Text("SDL Version: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);

		ImGui::Separator();

		ImGui::Text("CPUs:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "%i (Cache: %iKB)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

		ImGui::Text("System RAM:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "%iGB", SDL_GetSystemRAM() / 1000);

		//CPU Capabilities
		ImGui::Text("Caps: ");
		ImGui::SameLine();

		if (SDL_HasRDTSC())
		{
			ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "RDTSC");
			ImGui::SameLine();
		}
		if (SDL_HasAltiVec())
		{
			ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "Altivec");
			ImGui::SameLine();
		}
		if (SDL_Has3DNow())
		{
			ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "3DNow");
			ImGui::SameLine();
		}
		if (SDL_HasMMX())
		{
			ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "MMX");
			ImGui::SameLine();
		}
		if (SDL_HasSSE())
		{
			ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "SSE");
			ImGui::SameLine();
		}
		if (SDL_HasSSE2())
		{
			ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "SSE2");
			ImGui::SameLine();
		}
		if (SDL_HasSSE3())
		{
			ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "SSE3");
			ImGui::SameLine();
		}
		if (SDL_HasSSE41())
		{
			ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "SSE41");
			ImGui::SameLine();
		}
		if (SDL_HasSSE42())
		{
			ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "SSE42");
			ImGui::SameLine();
		}
		if (SDL_HasAVX())
		{
			ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "AVX");
			ImGui::SameLine();
		}
		ImGui::NewLine();

		ImGui::Separator();

		ImGui::Text("GPU:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "%s", (const char*)glGetString(GL_RENDERER));

		ImGui::Text("GPU Vendor:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "%s", (const char*)glGetString(GL_VENDOR));

		ImGui::Text("VRAM Budget:");
		ImGui::SameLine();
		int totalVRAM = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &totalVRAM);
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "%i", totalVRAM / 1000);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "MB");

		ImGui::Text("VRAM available:");
		ImGui::SameLine();
		int currentVRAM = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &currentVRAM);
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "%i", currentVRAM / 1000);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "MB");

		ImGui::Text("VRAM usage:");
		ImGui::SameLine();
		int VRAMUsage = totalVRAM - currentVRAM;
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "%i", VRAMUsage / 1000);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.90f, 0.70f, 0.00f, 1.0f), "MB");
	}
}