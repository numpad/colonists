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

static inline void onPressRenderWireframe(GLFWwindow *win, int key) {
	glPolygonMode(GL_FRONT_AND_BACK,
		(glfwGetKey(win, key) == GLFW_PRESS) ? GL_LINE : GL_FILL);
}

float deltaScroll = 0.0f;
static void onScrollSetGlobal(GLFWwindow *window, double xoff, double yoff) {
	deltaScroll = yoff;
}

int main(int argc, char *argv[]) {
	printf("version: %d.%d\n", CFG_VERSION_MAJOR, CFG_VERSION_MINOR);
	
	Window::Init();
	Window window(800, 600, "main");
	
	if (gl3wInit()) {
		fprintf(stderr, "gl3wInit() failed.\n");
	}
	printf("OpenGL %s, GLSL %s\n",
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	/* glfw setup */
	int cw, ch, cch;
	GLubyte *cdata = stbi_load("res/images/cursor/pointer.png", &cw, &ch, &cch, 0);
	GLFWimage img;
	img.width = img.height = cw;
	img.pixels = cdata;
	GLFWcursor *cursor = glfwCreateCursor(&img, 0, 0);
	glfwSetCursor(window, cursor);
	glfwSetScrollCallback(window, onScrollSetGlobal);
	
	Tilemap tilemap(500, 500);
	
	SimpleMapGenerator mgen;
	
	double begin_s = glfwGetTime();
	mgen.generate(tilemap);
	double end_s = glfwGetTime();
	double dt = end_s - begin_s;
	printf("generating world took %g ms.\n", dt * 1000.0f);

	glm::mat3 mView(
		1, 0, 0, //window.mouse.x * 2.0 - window.width,
		0, 1, 0, //window.mouse.y * -2.0 + window.height,
		0, 0, 1
	);
	
	while (window.isOpen()) {
		glfwPollEvents();
		
		/* debug */
		onPressRenderWireframe(window, GLFW_KEY_R);
		
		/* update */
		static glm::vec2 mouse, pMouse, dMouse; // current, prev, delta
		static double mDX, mDY;
		glfwGetCursorPos(window, &mDX, &mDY);
		mouse.x = mDX;
		mouse.y = mDY;
		
		dMouse = mouse - pMouse;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			mView[0][2] += dMouse.x * 2.0f;
			mView[1][2] += dMouse.y * 2.0f;
		}
		
		float zoomFactor = 1.0f + deltaScroll * 0.1f;
		mView[0][0] *= zoomFactor;
		mView[1][1] *= zoomFactor;
		
		
		/* draw */
		glClearColor(0.3, 0.3, 0.45, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
		
		tilemap.draw(window.getProjection(), mView);
		
		window.display();
		
		/* after frame stuff */
		pMouse = mouse;
		deltaScroll = 0.0f;
	}
		
	stbi_image_free(cdata);
	glfwDestroyCursor(cursor);
	window.close();
	glfwTerminate();
	return 0;
}
