#include "config.hpp"

#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include "GLWrapper/sgl_shader.hpp"
#include "GLWrapper/Texture.hpp"

#include "System/Window.hpp"

struct Mesh {
	GLuint vao, vbo, indices;
	
	void destroy() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &indices);
	}
	
};

Mesh createMesh(GLfloat *vertices, GLuint vlen, GLuint *indices, GLuint ilen) {
	Mesh m;
	
	glGenVertexArrays(1, &m.vao);
	glGenBuffers(1, &m.vbo);
	glGenBuffers(1, &m.indices);
	
	glBindVertexArray(m.vao);
		glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.indices);
		
		glBufferData(GL_ARRAY_BUFFER, vlen * sizeof(GLfloat), (void *)vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ilen * sizeof(GLuint), (void *)indices, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	return m;
}

int main(int argc, char *argv[]) {
	printf("version: %d.%d\n", CFG_VERSION_MAJOR, CFG_VERSION_MINOR);
	
	Window::Init();
	Window window(800, 600, "main");
	
	if (gl3wInit()) {
		fprintf(stderr, "gl3wInit() failed.\n");
	}
	if (!gl3wIsSupported(3, 3)) {
		fprintf(stderr, "OpenGL 3.3 not supported.\n");
	}
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	GLfloat vertices[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1
	};
	GLuint indices[] = {
		0, 1, 2, 2, 1, 3
	};
	
	Texture texture;
	texture.load("res/tileset.png");
	
	Mesh screen = createMesh(vertices, 8, indices, 6);
	
	sgl::shader shader;
	shader.load_from_memory(R"(
		#version 330 core
		
		uniform mat3 uProjection;
		uniform mat3 uView;
		
		layout (location = 0) in vec2 aPos;
		out vec2 vPos;
		
		void main() {
			vPos = aPos;
			gl_Position.xyz = uProjection * uView * vec3(aPos, 1.0);
			gl_Position.w = 1.0;
		}
	)", sgl::shader::VERTEX);
	
	shader.load_from_memory(R"(
		#version 330 core
		uniform sampler2D uTexture;
		
		in vec2 vPos;
		out vec4 FragColor;
		
		void main() {
			vec4 tex = texture(uTexture, vPos * 0.5 + 0.5);
			FragColor = vec4(tex.rgb, tex.a);
		}
	)", sgl::shader::FRAGMENT);
	shader.compile();
	shader.link();
	
	while (window.isOpen()) {
		glClearColor(0.3, 0.3, 0.45, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
		
		glm::mat3 mView(
			120, 0, window.mouse.x * 2.0 - window.width,
			0, 120, window.mouse.y * -2.0 + window.height,
			0, 0, 1
		);
		
		shader.use();
		shader["uView"] = glm::transpose(mView);
		shader["uProjection"] = window.getProjection();
		shader["uTexture"] = 0;
		
		texture.active(0);
		texture.bind(true);
		glBindVertexArray(screen.vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen.indices);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		texture.bind(false);
		
		
		window.display();
		glfwPollEvents();
	}
	
	texture.destroy();
	screen.destroy();
	
	window.close();
	glfwTerminate();
	return 0;
}
