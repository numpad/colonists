#include "RenderStates/GameState.hpp"

GameState::GameState()
	: map(128, 128)
{
	
}

GameState::~GameState() {
	
}

void GameState::onRender(sf::RenderTarget &target) {
	target.clear();
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		map.move(0,  10);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		map.move(0, -10);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		map.move( 10,  0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		map.move(-10,  0);
	
	
	map.setColor(mouseX / map.getTilesize(), mouseY / map.getTilesize(), sf::Color::Red);
	target.draw(map);
	map.setColor(mouseX / map.getTilesize(), mouseY / map.getTilesize(), sf::Color::White);
}

void GameState::onEvent(sf::Event &event) {
	switch (event.type) {
		case sf::Event::MouseMoved:
			this->mouseX = event.mouseMove.x;
			this->mouseY = event.mouseMove.y;
			break;
		default: break;
	};
}
