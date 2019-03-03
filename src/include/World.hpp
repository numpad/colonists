#ifndef WORLD_HPP
#define WORLD_HPP

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include "Tilemap.hpp"
#include "Assets/AssetManager.hpp"

class World : public sf::Drawable, public sf::Transformable {
	
	Tilemap groundLayer;
	
	AssetManager assets;
public:
	
	World(int w, int h);
	
	glm::vec2 getCenterPoint();
	
	virtual void draw(sf::RenderTarget &, sf::RenderStates) const;
};

#endif
