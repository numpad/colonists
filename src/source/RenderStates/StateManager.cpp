#include "RenderStates/StateManager.hpp"

StateManager::StateManager() {
	
}

StateManager::~StateManager() {
	for (size_t i = 0; i < this->loaded_states.size(); ++i) {
		delete this->loaded_states.at(i);
	}
}

bool StateManager::load(State *state) {
	size_t idx = this->loaded_states.size();
	this->loaded_states.push_back(state);
	this->state_queue.push(idx); 
}

void StateManager::handleEvent(sf::Event &event) {
	State *current = this->activeState();
	current->onEvent(event);
}

void StateManager::render(sf::RenderTarget &target) {
	State *current = this->activeState();
	current->onRender(target);
}

State *StateManager::activeState() const {
	return this->loaded_states.at(this->state_queue.top());
}