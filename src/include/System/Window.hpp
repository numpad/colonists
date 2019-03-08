#ifndef SYSTEM_WINDOW_HPP
#define SYSTEM_WINDOW_HPP value

#include <string>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window {
	
	GLFWwindow *window = nullptr;
	glm::mat3 projection;
	
	void registerCallbacks();
	
public:
	int width, height;
	static bool Init();
	
	Window(int width, int height, std::string title, bool fullscreen = false);
	~Window();
	
	bool isOpen();
	
	void setProjectionMatrix(int width, int height);
	glm::mat3& getProjection();
	
	void resize(int width, int height);
	void close();
	
	void display();
	
	operator GLFWwindow*() const {
		return window;
	}
};

#endif
