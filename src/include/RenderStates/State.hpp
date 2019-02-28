#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics.hpp>

class State {
	
public:
	virtual ~State() {};
	
	virtual void onRender(sf::RenderTarget &) =0;
	
	virtual void onEvent(sf::Event &) =0;
};

#endif
