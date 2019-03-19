#ifndef STATES_STATE_HPP
#define STATES_STATE_HPP

#include "System/Window.hpp"

class State {

public:
	
	virtual ~State();
	
	virtual void draw() =0;
};

#endif
