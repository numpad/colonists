#ifndef STATES_STATE_HPP
#define STATES_STATE_HPP

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "System/Window.hpp"
#include "System/ImUtil.hpp"

class State {
	bool finished = false;
	
public:
	
	virtual ~State();
	
	virtual void draw() =0;
	
	void setFinished(bool done = true);
	void drawUntilFinished(Window &window);
};

#endif
