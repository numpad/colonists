#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "config.hpp"

#include "RenderStates/State.hpp"
#include "RenderStates/GameState.hpp"

int main(int argc, char *argv[]) {
	
	printf("version: %d.%d\n", CFG_VERSION_MAJOR, CFG_VERSION_MINOR);
	
	sf::RenderWindow window(
		sf::VideoMode(800, 600),
		"main"
	);
	window.setPosition(sf::Vector2i(
		sf::VideoMode::getDesktopMode().width  * 0.5 - window.getSize().x * 0.5,
		sf::VideoMode::getDesktopMode().height * 0.5 - window.getSize().y * 0.5
	));
	
	State *state = new GameState();
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			state->onEvent(event);
			
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		state->onRender(window);
		window.display();
	}
	
	delete state;
	
	return 0;
}
