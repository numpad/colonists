#include "RenderStates/GameState.hpp"

GameState::GameState()
	: map(128, 128), camera(sf::Vector2f(0, 0), sf::Vector2f(800, 600))
{
	
}

GameState::~GameState() {
	
}

void GameState::onRender(sf::RenderTarget &target) {
	target.clear();
	target.setView(camera);
	
	auto pc = target.mapPixelToCoords(sf::Vector2i(mouseX, mouseY));
	glm::vec2 mouse_world(pc.x, pc.y);
	int tileX, tileY;
	map.mapToTile(mouse_world, &tileX, &tileY);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		camera.move(0, -10);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		camera.move(0,  10);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		camera.move(-10,  0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		camera.move( 10,  0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		camera.zoom(1.1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		camera.zoom(0.9);
	
	map.setColor(tileX, tileY, sf::Color::Red);
	target.draw(map);
	map.setColor(tileX, tileY, sf::Color::White);
}

void GameState::onEvent(sf::Event &event) {
	switch (event.type) {
		case sf::Event::MouseMoved:
			this->mouseX = event.mouseMove.x;
			this->mouseY = event.mouseMove.y;
			break;
		case sf::Event::Resized:
			camera.setSize(event.size.width, event.size.height);
			break;
		default: break;
	};
}
