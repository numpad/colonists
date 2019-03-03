#include "RenderStates/GameState.hpp"

GameState::GameState(sf::RenderWindow &window)
	: window(window), world(128, 128),
	camera(sf::Vector2f(
		world.getCenterPoint().x,
		world.getCenterPoint().y
	), sf::Vector2f(800, 600))
{
	
	
}

GameState::~GameState() {
	
}

glm::vec2 GameState::readCameraMoveInput() {
	glm::vec2 dir(0.0f, 0.0f);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		dir.y -= 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		dir.y += 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		dir.x -= 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		dir.x += 1.0f;
	
	if (glm::length2(dir) < 0.01)
		return dir;
	return glm::normalize(dir);
}

void GameState::onRender(sf::RenderTarget &target) {
	target.clear();
	target.setView(camera);
	
	sf::Vector2f pc = target.mapPixelToCoords(sf::Vector2i(mouse.x, mouse.y));
	glm::vec2 mouse_world(pc.x, pc.y);
	int tileX, tileY;
	//map.mapToTile(mouse_world, &tileX, &tileY);
	
	glm::vec2 camMove = readCameraMoveInput() * 22.5f;
	camera.move(camMove.x, camMove.y);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		camera.zoom(1.1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		camera.zoom(0.9);
	
	camera.setCenter((int)camera.getCenter().x, (int)camera.getCenter().y);
	camera.setSize((int)camera.getSize().x, (int)camera.getSize().y);
	target.draw(world);
	
	prevMouse = mouse;
}

void GameState::onEvent(sf::Event &event) {
	switch (event.type) {
		case sf::Event::MouseMoved:
			this->mouse.x = event.mouseMove.x;
			this->mouse.y = event.mouseMove.y;
			break;
		case sf::Event::Resized:
			camera.setSize(event.size.width, event.size.height);
			break;
		case sf::Event::MouseWheelScrolled: {
			// delta zoom
			camera.zoom(1.0f - event.mouseWheelScroll.delta * 0.1f);
			
			break;
		}
		default: break;
	};
}
