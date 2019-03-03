#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <SFML/Graphics.hpp>

#include "RenderStates/State.hpp"
#include "World.hpp"

class GameState : public State {
	sf::RenderWindow &window;
	
	World world;
	glm::vec2 mouse, prevMouse;
		
	sf::View camera;
	
	glm::vec2 readCameraMoveInput();
	
public:
	
	GameState(sf::RenderWindow &);
	~GameState();
	
	virtual void onRender(sf::RenderTarget &);
	virtual void onEvent(sf::Event &);
};

#endif
