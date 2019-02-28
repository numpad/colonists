#include "Tilemap.hpp"

static inline void make_square_vertex(sf::Vertex *vertices, int tile_id, int tilesize, int x, int y) {
	x *= tilesize;
	y *= tilesize;
	vertices[0].position = sf::Vector2f(x, y);
	vertices[1].position = sf::Vector2f(x + tilesize, y);
	vertices[2].position = sf::Vector2f(x + tilesize, y + tilesize);
	vertices[3].position = sf::Vector2f(x, y + tilesize);
	
	// tileset tilesize = 64
	int tu = tile_id % (256 / 64);
	int tv = tile_id / (256 / 64);
	
	tu *= 64;
	tv *= 64;
	
	vertices[0].texCoords = sf::Vector2f(tu, tv);
	vertices[1].texCoords = sf::Vector2f(tu + 64, tv);
	vertices[2].texCoords = sf::Vector2f(tu + 64, tv + 64);
	vertices[3].texCoords = sf::Vector2f(tu, tv + 64);
	
	for (int i = 0; i < 4; ++i) {
		vertices[i].color = sf::Color::White;
	}
}

Tilemap::Tilemap(int w, int h)
	: width(w), height(h), tilesize(48),
	mapVertices(sf::Quads, w * h * 4)
{
	FastNoise noise;
	noise.SetSeed(time(NULL));
	noise.SetNoiseType(FastNoise::Perlin);
	
	if (!this->tileset.loadFromFile("res/tileset.png")) {
		printf("failed loading tileset!\n");
	}
	
	for (int y = 0; y < this->height; ++y) {
		for (int x = 0; x < this->width; ++x) {
			for (int i = 0; i < 6; ++i) {
				// current vertex data
				sf::Vertex *cell = this->getVertices(x, y);
				
				int tile = 0;
				if (noise.GetNoise(x*15, y*15) > 0.12) tile = 1;
				else if (noise.GetNoise(x*30, y*30) > 0.25) tile = 2;
				
				make_square_vertex(cell, tile, tilesize, x, y);
			}
		}
	}
}
	
int Tilemap::getTilesize() const {
	return this->tilesize;
}

int Tilemap::getWidth() const {
	return this->width;
}

int Tilemap::getHeight() const {
	return this->height;
}

void Tilemap::setColor(int x, int y, sf::Color c) {
	sf::Vertex *v = this->getVertices(x, y);
	for (int i = 0; i < 4; ++i)
		v[i].color = c;
}


sf::Vertex *Tilemap::getVertices(int x, int y) {
	return &this->mapVertices[(x + y * this->width) * 4];
}


void Tilemap::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform *= this->getTransform();
	states.texture = &this->tileset;
	
	target.draw(mapVertices, states);
}
