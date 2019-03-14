#include "config.hpp"

#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <FastNoise/FastNoise.h>
#include <stb/stb_image.h>

#include "GLWrapper/sgl_shader.hpp"
#include "GLWrapper/Texture.hpp"

#include "System/Window.hpp"
#include "Graphics/Tilemap.hpp"
#include "MapGen/MapGen.hpp"
#include "MapGen/SimpleMapGen.hpp"

#include "System/ImUtil.hpp"

static inline void onPressRenderWireframe(GLFWwindow *win, int key) {
	glPolygonMode(GL_FRONT_AND_BACK,
		(glfwGetKey(win, key) == GLFW_PRESS) ? GL_LINE : GL_FILL);
}

float deltaScroll = 0.0f;
static void onScrollSetGlobal(GLFWwindow *, double xoff, double yoff) {
	(void)xoff;
	
	deltaScroll = yoff;
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	
	printf("version: %d.%d\n", CFG_VERSION_MAJOR, CFG_VERSION_MINOR);
	
	Window::Init();
	Window window(800, 600, "main");
	
	if (gl3wInit()) {
		fprintf(stderr, "gl3wInit() failed.\n");
	}
	printf("OpenGL %s, GLSL %s\n",
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	/* imgui setup */
	ImUtil::Init(window);
	
	/* glfw setup */
	int cw, ch, cch;
	GLubyte *cdata = stbi_load("res/images/cursor/pointer.png", &cw, &ch, &cch, 0);
	GLFWimage img;
	img.width = img.height = cw;
	img.pixels = cdata;
	GLFWcursor *cursor = glfwCreateCursor(&img, 0, 0);
	glfwSetCursor(window, cursor);
	glfwSetScrollCallback(window, onScrollSetGlobal);
	
	Tilemap tilemap(250, 250); // testing 2000
	
	SimpleMapGenerator mgen;
	
	double begin_s = glfwGetTime();
	mgen.generate(tilemap);
	double end_s = glfwGetTime();
	double dt = end_s - begin_s;
	printf("Generating world with seed %lu took %g ms.\n", mgen.getSeed(), dt * 1000.0f);
	//tilemap.clearCache();
	
	while (window.isOpen()) {
		glfwPollEvents();
		
		/* debug */
		ImUtil::PrepareFrame();
		
		onPressRenderWireframe(window, GLFW_KEY_R);
		
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
		static bool place_mode = false;
		static int tid = 0;
		
		/* draw imgui */
		if (ImUtil::Enabled()) {
			if (ImGui::Begin("Info")) {
				ImGui::Text("Mouse: %.1f, %.1f\n", mouseWorld.x, mouseWorld.y);
				ImGui::Text(" As Tile: %d, %d\n", mouseTile.x, mouseTile.y);
				
				ImGui::Separator();
				
				ImGui::InputInt("ID", &tid);
				ImGui::SameLine();
				if (ImGui::Checkbox("Place ID", &place_mode)) {
					//tilemap.setTileID(tilepos[0], tilepos[1], tid);
					//mgen.blend(tilemap);
					//tilemap.updateTileIDs();
				}
				
			}
			ImGui::End();
		}
		
		if (place_mode && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			tilemap.setTileID(mouseTile.x, mouseTile.y, tid);
			mgen.blend(tilemap);
			tilemap.updateTileIDs();
		}
		
		/* draw */
		glClearColor(0.35, 0.3, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
		
		tilemap.draw();
		ImUtil::RenderFrame();
		window.display();
		
		
		/* after frame stuff */
		pMouse = mouse;
		deltaScroll = 0.0f;
	}
	
	ImUtil::Destroy();
	stbi_image_free(cdata);
	glfwDestroyCursor(cursor);
	window.close();
	glfwTerminate();
	return 0;
}
