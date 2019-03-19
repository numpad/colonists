#include "System/ImUtil.hpp"

void ImUtil::Init(Window &window) {
#if CFG_IMGUI_ENABLED
	printf("Running with imgui %s.\n",
		ImUtil::Enabled() ? "enabled" : "disabled");
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
#endif
}

void ImUtil::PrepareFrame() {
#if CFG_IMGUI_ENABLED
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
#endif
}

void ImUtil::RenderFrame() {
#if CFG_IMGUI_ENABLED
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

void ImUtil::Destroy() {
#if CFG_IMGUI_ENABLED
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
#endif
}

bool ImUtil::Enabled() {
#if CFG_IMGUI_ENABLED
	return true;
#else
	return false;
#endif
}
