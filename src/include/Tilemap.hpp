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
	int tilesize, tilesetSize;
	
	sf::VertexArray mapVertices;
	sf::Texture tileset;
	
	sf::Vertex *getVertices(int x, int y);
	void make_square_vertex(sf::Vertex *vertices, int tile_id, int x, int y);
	
	bool validTilePos(int x, int y);
public:
	
	Tilemap(int width, int height);
	
	int getTilesize() const;
	int getWidth() const;
	int getHeight() const;
	
	// Maps a point in worldspace to a tile.
	void mapToTile(glm::vec2 point, int *tx, int *ty);
	
	void setColor(int x, int y, sf::Color c);
	void setTileID(int x, int y, int tile_id);
	
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
};

#endif
