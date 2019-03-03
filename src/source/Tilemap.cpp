#include "Tilemap.hpp"

void Tilemap::make_square_vertex(sf::Vertex *vertices, int tile_id, int x, int y) {
	int tx = x * tilesize;
	int ty = y * tilesize;
	vertices[0].position = sf::Vector2f(tx, ty);
	vertices[1].position = sf::Vector2f(tx + tilesize, ty);
	vertices[2].position = sf::Vector2f(tx + tilesize, ty + tilesize);
	vertices[3].position = sf::Vector2f(tx, ty + tilesize);

	this->setTileID(x, y, tile_id);
	this->setTileColor(x, y, sf::Color::White);
}

Tilemap::Tilemap(int w, int h)
	: width(w), height(h), tilesize(96),
	tilesetCols(10), tilesetRows(10), tilesetSize(96),
	mapVertices(sf::Quads, w * h * 4)
{
	FastNoise noise, bnoise;
	noise.SetSeed(time(NULL));
	noise.SetNoiseType(FastNoise::Perlin);
	noise.SetFrequency(0.005);
	bnoise.SetSeed(time(NULL) + 13);
	bnoise.SetNoiseType(FastNoise::Perlin);
	bnoise.SetFrequency(0.05);
	
	for (int y = 0; y < this->height; ++y) {
		for (int x = 0; x < this->width; ++x) {
			for (int i = 0; i < 6; ++i) {
				// current vertex data
				sf::Vertex *cell = this->getVertices(x, y);
				
				const int GRASS = tileCoordsToID(0 + rand() % 3, 0);
				const int DIRT  = tileCoordsToID(0 + rand() % 2, 1);
				const int SAND  = tileCoordsToID(0, 2);
				const int WATER = tileCoordsToID(0, 4);
				
				int tile = DIRT;
				if (noise.GetNoise(x*8, y*7.8) > 0.48 + bnoise.GetNoise(x * 16, y * 16) * - 0.07f) tile = WATER + 1;
				else if (noise.GetNoise(x*8, y*7.8) > 0.28) tile = WATER;
				else if (noise.GetNoise(x * 4.05, y * 3.98) > bnoise.GetNoise(x, y)) tile = GRASS;
				else if (noise.GetNoise(x*5.7, y*8) > 0.25) tile = SAND;
				
				this->make_square_vertex(cell, tile, x, y);
			}
		}
	}
	
	tileTransition.loadFromMemory(
		R"(
			
			void main() {
				gl_Position = gl_ModelViewMatrix[0] * gl_Vertex;
			}
		)",
		R"(
			void main() {
				gl_FragColor = gl_Color;
			}
		)"
	);
}

int Tilemap::tileCoordsToID(int tx, int ty) {
	return tx + ty * tilesetCols;
}

void Tilemap::setTileset(sf::Texture *texture, int cols, int rows, int tilesetSize) {
	this->tileset = texture;
	this->tilesetCols = cols;
	this->tilesetRows = rows;
	this->tilesetSize = tilesetSize;
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

void Tilemap::setTileColor(int x, int y, sf::Color c) {
	if (!this->validTilePos(x, y)) return;
	
	sf::Vertex *v = this->getVertices(x, y);
	for (int i = 0; i < 4; ++i)
		v[i].color = c;
}

void Tilemap::setTileID(int x, int y, int tile_id) {
	if (!this->validTilePos(x, y)) return;
	
	int tu = tile_id % (tilesetCols);
	int tv = tile_id / (tilesetRows);
	
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
	states.texture = this->tileset;
	
	target.draw(mapVertices, states);
}
