#include <SFML/Graphics.hpp>

int main(int argc, char *argv[]) {
	
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
