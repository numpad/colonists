#include "System/Window.hpp"

static void setWindowViewport(GLFWwindow *window, int w, int h) {
	Window *win = (Window *)glfwGetWindowUserPointer(window);
	
	glViewport(0, 0, w, h);
	win->setProjectionMatrix(w, h);
	win->width = w;
	win->height = h;
}

static void printError(int err, const char *desc) {
	fprintf(stderr, "Error #%d: %s\n", err, desc);
}

bool Window::Init() {
	if (!glfwInit()) {
		fprintf(stderr, "glfwInit() failed!\n");
		return false;
	}
	
	glfwSetErrorCallback(printError);
	
	return true;
}

Window::Window(int width, int height, std::string title, bool fullscreen)
	: width(width), height(height), projection(1.0f)
{
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(
		width, height,
		title.c_str(),
		fullscreen ? glfwGetPrimaryMonitor() : nullptr,
		nullptr);
	
	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window);
	
	glfwSwapInterval(1); // enable vsync
	
	registerCallbacks();
	
	setProjectionMatrix(width, height);
}

Window::~Window() {
}

void Window::setProjectionMatrix(int width, int height) {
	this->projection[0][0] = 1.0f / (float)width;
	this->projection[1][1] = -1.0f / (float)height;
	//projection[0][2] = -1.0f;
	//projection[1][2] = 1.0f;
}


bool Window::isOpen() {
	if (!window) return false;
	
	return !glfwWindowShouldClose(window);
}

glm::mat3& Window::getProjection() {
	return this->projection;
}

void Window::resize(int w, int h) {
	glfwSetWindowSize(window, w, h);
	this->width = w;
	this->height = h;
}

void Window::close() {
	glfwSetWindowShouldClose(window, true);
	glfwDestroyWindow(window);
}

void Window::display() {
	glfwSwapBuffers(window);
}

/// PRIVATE

void Window::registerCallbacks() {
	if (!window) return;
	glfwSetFramebufferSizeCallback(window, setWindowViewport);
}
