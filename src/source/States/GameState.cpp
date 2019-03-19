#include "States/GameState.hpp"


static float deltaScroll = 0.0f;
static void onScrollSetGlobal(GLFWwindow *, double xoff, double yoff) {
	(void)xoff;
	
	deltaScroll = yoff;
}


static inline void renderWireframe(GLFWwindow *win, bool enabled) {
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

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		
	}
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		tilemap.translate(dMouse);
	}
	
	float zoomFactor = 1.0f + deltaScroll * 0.1f;
	tilemap.scale(zoomFactor);
	tilemap.setProjectionMatrix(window.getProjection());
	
	glm::vec2 mouseWorld = tilemap.mapLocalToWorldCoords(window, mouse);
	glm::ivec2 mouseTile = tilemap.mapWorldToTileCoords(mouseWorld);
	glm::vec2 tileWorld = tilemap.mapTileToWorldCoords(mouseTile);
	static bool place_mode = false, wireframe = false;
	static int tid = 0;
	
	/* draw imgui */
	if (ImUtil::Enabled()) {
		if (ImGui::Begin("Tilemap")) {
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
	}
	
	renderWireframe(window, wireframe);
	if (place_mode && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
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