#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include "FastNoise/FastNoise.h"

class Tilemap : public sf::Drawable, public sf::Transformable {
	// map tile(s) data
	int width, height;
	int tilesize;
	
	// tileset data
	int tilesetCols, tilesetRows;
	int tilesetSize;
	
	sf::Shader tileTransition;
	sf::Texture *tileset;
	sf::VertexArray mapVertices;
	
	sf::Vertex *getVertices(int x, int y);
	void make_square_vertex(sf::Vertex *vertices, int tile_id, int x, int y);
	
	bool validTilePos(int x, int y);
	int tileCoordsToID(int tx, int ty);
	
public:
	
	Tilemap(int width, int height);
	
	void setTileset(sf::Texture *texture, int cols, int rows, int tilesetSize);
	
	int getTilesize() const;
	int getWidth() const;
	int getHeight() const;
	
	// Maps a point in worldspace to a tile.
	void mapToTile(glm::vec2 point, int *tx, int *ty);
	
	void setTileColor(int x, int y, sf::Color c);
	void setTileID(int x, int y, int tile_id);
	
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
};

#endif
