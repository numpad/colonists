#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "config.hpp"

#include "RenderStates/StateManager.hpp"
#include "RenderStates/GameState.hpp"

#include "Tilemap.hpp"

int main(int argc, char *argv[]) {
	
	printf("version: %d.%d\n", CFG_VERSION_MAJOR, CFG_VERSION_MINOR);
	
	sf::ContextSettings ctx(0, 0, 0, 3, 0);
	
	sf::RenderWindow window(
		sf::VideoMode(800, 600),
		"main",
		sf::Style::Default,
		ctx
	);
	window.setPosition(sf::Vector2i(
		sf::VideoMode::getDesktopMode().width  * 0.5 - window.getSize().x * 0.5,
		sf::VideoMode::getDesktopMode().height * 0.5 - window.getSize().y * 0.5
	));
	window.setVerticalSyncEnabled(true);
	
	StateManager smanager;
	smanager.load(new GameState(window));
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			smanager.handleEvent(event);
			
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized) {
				
			}
		}
		
		
		smanager.render(window);
		window.display();
	}
	
	return 0;
}
