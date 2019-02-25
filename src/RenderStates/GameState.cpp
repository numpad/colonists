#include "RenderStates/GameState.hpp"

GameState::~GameState() {
	
}

void GameState::onCreate() {
	
}

void GameState::onRender(sf::RenderTarget &target) {
	target.clear(sf::Color::Green);
	
}

void GameState::onEvent(sf::Event &event) {
	
}
