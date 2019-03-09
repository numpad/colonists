#include "Graphics/Tilemap.hpp"

static inline size_t tileToIndex(int tx, int ty, int w, int h) {
	(void)h; // TODO: check if inside bounds
	return tx + ty * w;
}

static inline void indexToTile(size_t index, int *tx, int *ty, int w, int h) {
	(void)h; // TODO: check if inside bounds
	*tx = index % w;
	*ty = index / w;
}

Tilemap::Tilemap(int w, int h):
	width(w), height(h),
	tiledrawer("res/glsl/tilemap/drawsimple.vert.glsl", "res/glsl/tilemap/drawsimple.frag.glsl"),
	tileVertexData(w * h * 6),
	tileIDs(w * h),
	tileTexCoordData(w * h * 6)
{
	this->loadTileset("res/tileset.png");
	this->loadTileVertices();
}

Tilemap::~Tilemap() {
	tileset->destroy();
	delete tileset;
	destroyTileVertices();
}

void Tilemap::getSize(int *cols, int *rows) {
	if (cols) *cols = width;
	if (rows) *rows = height;
}

void Tilemap::setTileID(int x, int y, int id) {
	int tx, ty;
	indexToTile(id, &tx, &ty, tilesetSize, tilesetSize);
	
	setTileTexCoords(x, y, glm::vec2((float)tx, (float)ty));
	glBindBuffer(GL_ARRAY_BUFFER, tilesTexCoords);
	glBufferData(GL_ARRAY_BUFFER,
		tileTexCoordData.size() * 2 * sizeof(GLfloat),
		&tileTexCoordData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Tilemap::loadTileset(std::string path) {
	if (tileset) {
		tileset->destroy();
		delete tileset;
	}
	tileset = new Texture();
	if (!tileset->load(path)) {
		fprintf(stderr, "Error: could not load tileset!\n");
	}
	
}

void Tilemap::draw(glm::mat3 &uProjection, glm::mat3 &uView) {
	tiledrawer.use();
	tiledrawer["uProjection"] = glm::transpose(uProjection);
	tiledrawer["uView"] = glm::transpose(uView);
	tiledrawer["uTileset"] = 0;
	tiledrawer["uTilesize"] = (float)tilesize;
	
	
	tileset->active(0);
	tiledrawer.use();
	glBindVertexArray(tilesVAO);	
	glDrawArrays(GL_TRIANGLES, 0, width * height * 6);
	glBindVertexArray(0);
	
	
}


/// PRIVATE

int Tilemap::getTilesetSizeInPixels() {
	return tilesetTilesize * tilesetSize;
}

void Tilemap::setTileVertices(int tx, int ty) {
	int i = tileToIndex(tx, ty, width, height);
	
	glm::vec2 pos((float)tx * (float)tilesize, (float)ty * (float)tilesize);
	glm::vec2 tRight = glm::vec2((float)tilesize, 0.0f);
	glm::vec2 tDown = glm::vec2(0.0f, (float)tilesize);
	
	tileVertexData.at(i * 6 + 0) = pos;
	tileVertexData.at(i * 6 + 1) = pos + tRight;
	tileVertexData.at(i * 6 + 2) = pos + tDown;
	tileVertexData.at(i * 6 + 3) = pos + tRight;
	tileVertexData.at(i * 6 + 4) = pos + tRight + tDown;
	tileVertexData.at(i * 6 + 5) = pos + tDown;
}

void Tilemap::setTileTexCoords(int tx, int ty, glm::vec2 s) {
	float n_tilesize = 1.0f / (float)tilesetSize;
	int i = tileToIndex(tx, ty, width, height);
	
	tileTexCoordData.at(i * 6 + 0) = s * n_tilesize + glm::vec2(0.0f,        0.0f);
	tileTexCoordData.at(i * 6 + 1) = s * n_tilesize + glm::vec2(n_tilesize,  0.0f);
	tileTexCoordData.at(i * 6 + 2) = s * n_tilesize + glm::vec2(0.0f,        n_tilesize);
	tileTexCoordData.at(i * 6 + 3) = s * n_tilesize + glm::vec2(n_tilesize,  0.0f);
	tileTexCoordData.at(i * 6 + 4) = s * n_tilesize + glm::vec2(n_tilesize,  n_tilesize);
	tileTexCoordData.at(i * 6 + 5) = s * n_tilesize + glm::vec2(0.0f,        n_tilesize);
}

void Tilemap::loadTileVertices() {
	glGenVertexArrays(1, &tilesVAO);
	glGenBuffers(1, &tilesVBO);
	glGenBuffers(1, &tilesTexCoords);
	
	this->generateTileVertices();
	
	glBindVertexArray(tilesVAO);
		glBindBuffer(GL_ARRAY_BUFFER, tilesVBO);
		glBufferData(GL_ARRAY_BUFFER,
			tileVertexData.size() * 2 * sizeof(GLfloat),
			&tileVertexData[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, tilesTexCoords);
		glBufferData(GL_ARRAY_BUFFER,
			tileTexCoordData.size() * 2 * sizeof(GLfloat),
			&tileTexCoordData[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Tilemap::generateTileVertices() {
	
	// generate a grid for the tiles vertices
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			setTileVertices(x, y);
			setTileTexCoords(x, y, glm::vec2(0.0f, 0.0f));
		}
	}
}

void Tilemap::destroyTileVertices() {
	glDeleteVertexArrays(1, &tilesVAO);
	glDeleteBuffers(1, &tilesVBO);
	glDeleteBuffers(1, &tilesTexCoords);
}
