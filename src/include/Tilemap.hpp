#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include "FastNoise/FastNoise.h"

class Tilemap : public sf::Drawable, public sf::Transformable {
	
	int width, height;
	int tilesize;
	
	sf::VertexArray mapVertices;
	sf::Texture tileset;
	
	sf::Vertex *getVertices(int x, int y);
	
public:
	
	Tilemap(int width, int height);
	
	int getTilesize() const;
	int getWidth() const;
	int getHeight() const;
	
	void setColor(int x, int y, sf::Color c);
	
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
};

#endif
