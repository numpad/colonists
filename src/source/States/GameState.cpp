#include "States/GameState.hpp"


static float deltaScroll = 0.0f;
static void onScrollSetGlobal(GLFWwindow *, double xoff, double yoff) {
	(void)xoff;
	
	deltaScroll = yoff;
}


static inline void renderWireframe(GLFWwindow *, bool enabled) {
	glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
}

GameState::GameState(Window &window, Tilemap &tilemap):
	window(window),
	tilemap(tilemap)
{
	glfwSetScrollCallback(window, onScrollSetGlobal);
}

GameState::~GameState() {
	
}

void GameState::draw() {
	/* update */
	static glm::vec2 mouse, pMouse, dMouse; // current, prev, delta
	static double mDX, mDY;
	glfwGetCursorPos(window, &mDX, &mDY);
	mouse.x = mDX;
	mouse.y = mDY;
	
	dMouse = mouse - pMouse;
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		tilemap.translate(dMouse);
	}
	
	float zoomFactor = 1.0f + deltaScroll * 0.1f;
	tilemap.scale(zoomFactor);
	tilemap.setProjectionMatrix(window.getProjection());
	
	glm::vec2 mouseWorld = tilemap.mapLocalToWorldCoords(window, mouse);
	glm::ivec2 mouseTile = tilemap.mapWorldToTileCoords(mouseWorld);
	glm::vec2 tileWorld = tilemap.mapTileToWorldCoords(mouseTile);
	
	static glm::ivec2 selectedTile(0);
	static bool place_mode = false, wireframe = false;
	static int tid = 0;
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
		&& !ImGui::GetIO().WantCaptureMouse)
	{
		selectedTile = mouseTile;
	}
	
	/* draw imgui */
	if (ImUtil::Enabled()) {
		static char titleFormatted[32] = {0};
		sprintf(titleFormatted, "Tilemap %dx%d##Tilemap",
			tilemap.getWidth(), tilemap.getHeight());
		
		if (ImGui::Begin(titleFormatted)) {
			ImGui::TextColored(ImVec4(0.92, 0.7, 0.0, 1.0), "FPS: %.1f", ImGui::GetIO().Framerate);
			ImGui::Text("Mouse: %.1f, %.1f\n", mouseWorld.x, mouseWorld.y);
			ImGui::Text("Tile: '%d' at (%d,%d).\n",
				tilemap.getTileID(mouseTile.x, mouseTile.y), mouseTile.x, mouseTile.y);
			ImGui::Text("Tile Pos: %g,%g\n", tileWorld.x, tileWorld.y);
			ImGui::Separator();
			
			ImGui::InputInt("ID", &tid);
			ImGui::SameLine();
			ImGui::Checkbox("Place ID", &place_mode);
			ImGui::Separator();
			
			ImGui::Checkbox("Wireframe", &wireframe);
			
		}
		ImGui::End();
		
		if (tilemap.inside(selectedTile.x, selectedTile.y)
			&& ImGui::Begin("Selected Tile"))
		{
			glm::vec2 *tileTexCoords = tilemap.getTileTexCoords(selectedTile.x, selectedTile.y);
			ImGui::Text("Selected Tile: (%d, %d)", selectedTile.x, selectedTile.y);
			ImGui::Text(" ID: %d", tilemap.getTileID(selectedTile.x, selectedTile.y));
			ImGui::Text("TexCoords:");
			ImGui::InputFloat2("Top-Left", (float *)(tileTexCoords) + 0 * 2);
			ImGui::InputFloat2("Top-Right", (float *)(tileTexCoords) + 1 * 2);
			ImGui::InputFloat2("Bottom-Left", (float *)(tileTexCoords) + 2 * 2);
			ImGui::InputFloat2("Bottom-Right", (float *)(tileTexCoords) + 4 * 2);
			tileTexCoords[3] = tileTexCoords[1];
			tileTexCoords[5] = tileTexCoords[2];
			
			if (ImGui::Button("Update")) {
				tilemap.updateTileIDs();
			}
			
		}
		ImGui::End();
		
	}
	
	renderWireframe(window, wireframe);
	
	// TODO: will not compile when imgui disabled
	if (place_mode && !ImGui::GetIO().WantCaptureMouse
		&& glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		tilemap.setTileID(mouseTile.x, mouseTile.y, tid);
		//mgen.blend(tilemap);
		tilemap.updateTileIDs();
	}
	
	/* draw */
	glClearColor(0.35, 0.3, 0.4, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	
	tilemap.draw();
	
	/* after frame stuff */
	pMouse = mouse;
	deltaScroll = 0.0f;
}
