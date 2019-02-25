#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/Graphics.hpp>

#include "RenderStates/State.hpp"

class GameState : public State {
	
public:
	
	~GameState();
	
	virtual void onCreate();
	virtual void onRender(sf::RenderTarget &);
	virtual void onEvent(sf::Event &);
};

#endif
