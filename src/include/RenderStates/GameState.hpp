#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/Graphics.hpp>

#include "RenderStates/State.hpp"
#include "Tilemap.hpp"

class GameState : public State {
	Tilemap map;
	float mouseX, mouseY;
	
	
public:
	
	GameState();
	~GameState();
	
	virtual void onRender(sf::RenderTarget &);
	virtual void onEvent(sf::Event &);
};

#endif
