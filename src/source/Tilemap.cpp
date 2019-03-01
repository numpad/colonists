#include "Tilemap.hpp"

void Tilemap::make_square_vertex(sf::Vertex *vertices, int tile_id, int x, int y) {
	int tx = x * tilesize;
	int ty = y * tilesize;
	vertices[0].position = sf::Vector2f(tx, ty);
	vertices[1].position = sf::Vector2f(tx + tilesize, ty);
	vertices[2].position = sf::Vector2f(tx + tilesize, ty + tilesize);
	vertices[3].position = sf::Vector2f(tx, ty + tilesize);
	
	// tileset tilesize = 64
	/*
	int tu = tile_id % (256 / tilesetSize);
	int tv = tile_id / (256 / tilesetSize);
	
	tu *= tilesetSize;
	tv *= tilesetSize;
	
	vertices[0].texCoords = sf::Vector2f(tu, tv);
	vertices[1].texCoords = sf::Vector2f(tu + tilesetSize, tv);
	vertices[2].texCoords = sf::Vector2f(tu + tilesetSize, tv + tilesetSize);
	vertices[3].texCoords = sf::Vector2f(tu, tv + tilesetSize);
	*/
	
	this->setTileID(x, y, tile_id);
	
	for (int i = 0; i < 4; ++i) {
		vertices[i].color = sf::Color::White;
	}
}

Tilemap::Tilemap(int w, int h)
	: width(w), height(h), tilesize(48), tilesetSize(64),
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
				if (noise.GetNoise(x*10, y*10) > 0.12) tile = 1;
				else if (noise.GetNoise(x*3, y*3) > 0.25) tile = 2;
				
				this->make_square_vertex(cell, tile, x, y);
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

void Tilemap::mapToTile(glm::vec2 point, int *tx, int *ty) {
	*tx = point.x / this->tilesize;
	*ty = point.y / this->tilesize;
}

bool Tilemap::validTilePos(int x, int y) {
	return (x >= 0 && y >= 0 && x < width && y < height);
}

void Tilemap::setColor(int x, int y, sf::Color c) {
	if (!this->validTilePos(x, y)) return;
	
	sf::Vertex *v = this->getVertices(x, y);
	for (int i = 0; i < 4; ++i)
		v[i].color = c;
}

void Tilemap::setTileID(int x, int y, int tile_id) {
	if (!this->validTilePos(x, y)) return;
	
	int tu = tile_id % (256 / tilesetSize);
	int tv = tile_id / (256 / tilesetSize);
	
	tu *= tilesetSize;
	tv *= tilesetSize;
	
	sf::Vertex *vertices = this->getVertices(x, y);
	vertices[0].texCoords = sf::Vector2f(tu, tv);
	vertices[1].texCoords = sf::Vector2f(tu + tilesetSize, tv);
	vertices[2].texCoords = sf::Vector2f(tu + tilesetSize, tv + tilesetSize);
	vertices[3].texCoords = sf::Vector2f(tu, tv + tilesetSize);
}

sf::Vertex *Tilemap::getVertices(int x, int y) {
	return &this->mapVertices[(x + y * this->width) * 4];
}


void Tilemap::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform *= this->getTransform();
	states.texture = &this->tileset;
	
	target.draw(mapVertices, states);
}
