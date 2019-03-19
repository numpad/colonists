#include "config.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <FastNoise/FastNoise.h>
#include <stb/stb_image.h>

#include "GLWrapper/sgl_shader.hpp"
#include "GLWrapper/Texture.hpp"

#include "System/Window.hpp"
#include "Graphics/Tilemap.hpp"

#include "System/ImUtil.hpp"
#include "States/MapGenState.hpp"
#include "States/GameState.hpp"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	srand(time(nullptr));
	
	printf("version: %d.%d\n", CFG_VERSION_MAJOR, CFG_VERSION_MINOR);
	
	Window::Init();
	Window window(800, 720, "main");
	
	if (gl3wInit()) {
		fprintf(stderr, "gl3wInit() failed.\n");
	}
	printf("OpenGL %s, GLSL %s\n",
		glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	/* imgui setup */
	ImUtil::Init(window);
	State *currentState = nullptr;
	
	/* glfw setup */
	int cw, ch, cch;
	GLubyte *cdata = stbi_load("res/images/cursor/pointer.png", &cw, &ch, &cch, 0);
	GLFWimage img;
	img.width = img.height = cw;
	img.pixels = cdata;
	GLFWcursor *cursor = glfwCreateCursor(&img, 0, 0);
	glfwSetCursor(window, cursor);
	
	Tilemap tilemap;
	
	currentState = new MapGenState(tilemap);
	currentState->drawUntilFinished(window);
	delete currentState;
	
	currentState = new GameState(window, tilemap);
	currentState->drawUntilFinished(window);
	delete currentState;
	
	ImUtil::Destroy();
	stbi_image_free(cdata);
	glfwDestroyCursor(cursor);
	window.close();
	glfwTerminate();
	printf("\ncleanup complete, quitting now...\n\n");
	return 0;
}
