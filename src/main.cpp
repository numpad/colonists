#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "config.hpp"

int main(int argc, char *argv[]) {
	
	printf("version: %d.%d\n", CFG_VERSION_MAJOR, CFG_VERSION_MINOR);
	
	sf::RenderWindow window(
		sf::VideoMode(800, 600),
		"colony"
	);
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
			};
		}
		
		window.clear(sf::Color::White);
		
		window.display();
	}
	
	return 0;
}
