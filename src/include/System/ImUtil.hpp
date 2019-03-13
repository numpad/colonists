#ifndef SYSTEM_IMUTIL_HPP
#define SYSTEM_IMUTIL_HPP

#include "config.hpp"

#include "System/Window.hpp"

#include <imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

class ImUtil {

public:
	
	static void Init(Window &window);
	static void PrepareFrame();
	static void RenderFrame();
	static void Destroy();
	
	static bool Enabled();
};

#endif
