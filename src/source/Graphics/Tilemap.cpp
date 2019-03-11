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
	tileTexCoordData(w * h * 6),
	blendTexCoordData(w * h * 6),
	overlapTexCoordData(w * h * 6)
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

int Tilemap::getTileID(int x, int y) {
	int i = tileToIndex(x, y, width, height);
	return tileIDs.at(i);
}

void Tilemap::setTileID(int x, int y, int id) {
	// tile on tileset texture
	int tex_x, tex_y;
	indexToTile(id, &tex_x, &tex_y, tilesetSize, tilesetSize);
	
	setTileTexCoords(tileTexCoordData, x, y, glm::vec2((float)tex_x, (float)tex_y));
	tileIDs.at(x + y * width) = id;
}

void Tilemap::setBlendTileID(int x, int y, int blend_id, int overlap_id) {
	// tile on tileset texture
	int blend_x, blend_y;
	int overlap_x, overlap_y;
	indexToTile(blend_id, &blend_x, &blend_y, tilesetSize, tilesetSize);
	indexToTile(overlap_id, &overlap_x, &overlap_y, tilesetSize, tilesetSize);
	
	setTileTexCoords(blendTexCoordData, x, y, glm::vec2((float)blend_x, (float)blend_y));
	setTileTexCoords(overlapTexCoordData, x, y, glm::vec2((float)overlap_x, (float)overlap_y));
}

void Tilemap::updateTileIDs() {
	glBindBuffer(GL_ARRAY_BUFFER, tilesTexCoords);
	glBufferSubData(GL_ARRAY_BUFFER,
		0, tileTexCoordData.size() * 2 * sizeof(GLfloat),
		&tileTexCoordData[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, blendTexCoords);
	glBufferSubData(GL_ARRAY_BUFFER,
		0, blendTexCoordData.size() * 2 * sizeof(GLfloat),
		&blendTexCoordData[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, overlapTexCoords);
	glBufferSubData(GL_ARRAY_BUFFER,
		0, overlapTexCoordData.size() * 2 * sizeof(GLfloat),
		&overlapTexCoordData[0]);
	
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

void Tilemap::draw() {
	tiledrawer.use();
	//tiledrawer["uProjection"] = glm::transpose(uProjection);
	//tiledrawer["uView"] = glm::transpose(uView);
	//tiledrawer["uModel"] = glm::transpose(uModel);
	tiledrawer["uTileset"] = 0;
	tiledrawer["uTilesize"] = (float)tilesize;
	
	
	tileset->active(0);
	tiledrawer.use();
	glBindVertexArray(tilesVAO);	
	glDrawArrays(GL_TRIANGLES, 0, width * height * 6);
	glBindVertexArray(0);
		
}

void Tilemap::setProjectionMatrix(glm::mat3 &uProj) {
	tiledrawer["uProjection"] = glm::transpose(uProj);
}

void Tilemap::setViewMatrix(glm::mat3 &uView) {
	tiledrawer["uView"] = glm::transpose(uView);
}

void Tilemap::setModelMatrix(glm::mat3 &uModel) {
	tiledrawer["uModel"] = glm::transpose(uModel);
}


/// PRIVATE

int Tilemap::getTilesetSizeInPixels() {
	return tilesetTilesize * tilesetSize;
}

void Tilemap::setTileVertices(int tx, int ty) {
	int i = tileToIndex(tx, ty, width, height);
	
	glm::vec2 pos(
		(float)tx * (float)tilesize,
		(float)ty * (float)tilesize);
	glm::vec2 off(
		(float)width * (float)tilesize * -0.5f,
		(float)height * (float)tilesize * -0.5f);
	
	glm::vec2 tRight = glm::vec2((float)tilesize, 0.0f);
	glm::vec2 tDown = glm::vec2(0.0f, (float)tilesize);
	
	tileVertexData.at(i * 6 + 0) = off + pos;
	tileVertexData.at(i * 6 + 1) = off + pos + tRight;
	tileVertexData.at(i * 6 + 2) = off + pos + tDown;
	tileVertexData.at(i * 6 + 3) = off + pos + tRight;
	tileVertexData.at(i * 6 + 4) = off + pos + tRight + tDown;
	tileVertexData.at(i * 6 + 5) = off + pos + tDown;
}

void Tilemap::setTileTexCoords(std::vector<glm::vec2> &target, int tx, int ty, glm::vec2 s) {
	float pixel = 0.0001 / getTilesetSizeInPixels(); // TODO: fix texcoord errors on zoom
	float n_tilesize = 1.0 / (double)tilesetSize;
	int i = tileToIndex(tx, ty, width, height);
	
	target.at(i * 6 + 0) = s * n_tilesize + glm::vec2(0.0f + pixel,        0.0f + pixel);
	target.at(i * 6 + 1) = s * n_tilesize + glm::vec2(n_tilesize - pixel,  0.0f + pixel);
	target.at(i * 6 + 2) = s * n_tilesize + glm::vec2(0.0f + pixel,        n_tilesize - pixel);
	target.at(i * 6 + 3) = s * n_tilesize + glm::vec2(n_tilesize - pixel,  0.0f + pixel);
	target.at(i * 6 + 4) = s * n_tilesize + glm::vec2(n_tilesize - pixel,  n_tilesize - pixel);
	target.at(i * 6 + 5) = s * n_tilesize + glm::vec2(0.0f + pixel,        n_tilesize - pixel);
}

void Tilemap::loadTileVertices() {
	glGenVertexArrays(1, &tilesVAO);
	glGenBuffers(1, &tilesVBO);
	glGenBuffers(1, &tilesTexCoords);
	glGenBuffers(1, &blendTexCoords);
	glGenBuffers(1, &overlapTexCoords);
	
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
		
		
		glBindBuffer(GL_ARRAY_BUFFER, blendTexCoords);
		glBufferData(GL_ARRAY_BUFFER,
			blendTexCoordData.size() * 2 * sizeof(GLfloat),
			&blendTexCoordData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, overlapTexCoords);
		glBufferData(GL_ARRAY_BUFFER,
			overlapTexCoordData.size() * 2 * sizeof(GLfloat),
			&overlapTexCoordData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Tilemap::generateTileVertices() {
	
	// generate a grid for the tiles vertices
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			setTileVertices(x, y);
			setTileTexCoords(tileTexCoordData, x, y, glm::vec2(0.0f, 0.0f));
			setTileTexCoords(blendTexCoordData, x, y, glm::vec2(0.0f, 0.0f));
			setTileTexCoords(overlapTexCoordData, x, y, glm::vec2(0.0f, 0.0f));
		}
	}
}

void Tilemap::destroyTileVertices() {
	glDeleteVertexArrays(1, &tilesVAO);
	glDeleteBuffers(1, &tilesVBO);
	glDeleteBuffers(1, &tilesTexCoords);
	glDeleteBuffers(1, &blendTexCoords);
	glDeleteBuffers(1, &overlapTexCoords);
}
