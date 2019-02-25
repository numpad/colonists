#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <vector>
#include <stack>
#include "RenderStates/State.hpp"

class StateManager {
	std::vector<State *> loaded_states;
	std::stack<size_t> state_queue;
	
public:
	
	StateManager();
	~StateManager();
	
	void handleEvent(sf::Event &);
	bool load(State *state);
	
	void render(sf::RenderTarget &);
	
	State *activeState() const;
};

#endif
