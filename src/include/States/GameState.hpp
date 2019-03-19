#ifndef STATES_GAMESTATE_HPP
#define STATES_GAMESTATE_HPP

#include "States/State.hpp"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "System/Window.hpp"
#include "System/ImUtil.hpp"
#include "Graphics/Tilemap.hpp"

class GameState : public State {

	Window &window;
	Tilemap &tilemap;
	
public:
	
	GameState(Window &window, Tilemap &tilemap);
	virtual ~GameState();
	
	virtual void draw();
};

#endif
