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

Tilemap::Tilemap(int w, int h) {
	create(w, h);
}

Tilemap::Tilemap():
	mView(1.0f),
	mModel(1.0f)
{
	
}

void Tilemap::create(int w, int h) {
	tiledrawer.load("res/glsl/tilemap/drawsimple.vert.glsl", sgl::shader::VERTEX);
	tiledrawer.load("res/glsl/tilemap/drawsimple.frag.glsl", sgl::shader::FRAGMENT);
	tiledrawer.compile();
	tiledrawer.link();
	
	width = w;
	height = h;
	tileIDs.resize(w * h);
	tileVertexData.resize(w * h * 6);
	tileTexCoordData.resize(w * h * 6);
	blendTexCoordData.resize(w * h * 6);
	overlapTexCoordData.resize(w * h * 6);
	mView = glm::mat3(1.0f);
	mModel = glm::mat3(1.0f);
	
	this->loadTileset("res/tileset.png");
	this->loadTileVertices();
	
	this->setModelMatrix(mModel);
	this->setViewMatrix(mView);
}

Tilemap::~Tilemap() {
	tileset->destroy();
	delete tileset;
	destroyTileVertices();
}

bool Tilemap::inside(int x, int y) {
	return (x >= 0 && y >= 0 && x < width && y < height);
}

void Tilemap::getSize(int *cols, int *rows) {
	if (cols) *cols = width;
	if (rows) *rows = height;
}

int Tilemap::getWidth() {
	return width;
}

int Tilemap::getHeight() {
	return height;
}

void Tilemap::translate(glm::vec2 offset) {
	mModel[0][2] += (offset.x * 2.0f) / mView[0][0];
	mModel[1][2] += (offset.y * 2.0f) / mView[1][1];
	setModelMatrix(mModel);
}

void Tilemap::scale(float s) {
	mView[0][0] *= s;
	mView[1][1] *= s;
	setViewMatrix(mView);
}

int Tilemap::getTileID(int x, int y) {
	if (x < 0 || y < 0 || x >= width || y >= height) return -1;
	
	int i = tileToIndex(x, y, width, height);
	return tileIDs.at(i);
}

glm::vec2 *Tilemap::getTileTexCoords(int x, int y) {
	return &tileTexCoordData.at(tileToIndex(x, y, width, height) * 6);
}

void Tilemap::setTileID(int x, int y, int id) {
	// tile on tileset texture
	int tex_x, tex_y;
	indexToTile(id, &tex_x, &tex_y, tilesetSize, tilesetSize);
	
	setTileTexCoords(tileTexCoordData, x, y, glm::vec2((float)tex_x, (float)tex_y));
	tileIDs.at(x + y * width) = id;
}

void Tilemap::setBlendTileID(int x, int y, int blend_id, int overlap_id, float angle) {
	// tile on tileset texture
	int blend_x, blend_y;
	int overlap_x, overlap_y;
	indexToTile(blend_id, &blend_x, &blend_y, tilesetSize, tilesetSize);
	indexToTile(overlap_id, &overlap_x, &overlap_y, tilesetSize, tilesetSize);
	
	setTileTexCoords(blendTexCoordData, x, y, glm::vec2((float)blend_x, (float)blend_y), angle);
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
	
	tileset->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tileset->set(GL_TEXTURE_WRAP_T, GL_REPEAT);
	
}

glm::vec2 Tilemap::mapLocalToWorldCoords(Window &window, glm::vec2 local) {
	glm::vec2 zoom(mView[0][0], mView[1][1]);
	glm::vec2 offset(mModel[0][2] * 0.5f, mModel[1][2] * 0.5f);
	glm::vec2 lcenter = glm::vec2(window.width, window.height) * 0.5f;
	glm::vec2 cmouse = (local - lcenter);
	
	glm::vec2 world = cmouse / zoom;
	return 2.0f * (world - offset);
}

glm::ivec2 Tilemap::mapWorldToTileCoords(glm::vec2 worldpos) {
	return glm::ivec2(
		(int)(worldpos.x + (width * tilesize) / 2) / tilesize,
		(int)(worldpos.y + (height * tilesize) / 2) / tilesize
	);
}

glm::vec2 Tilemap::mapTileToWorldCoords(glm::ivec2 tilepos) {
	return ((glm::vec2)tilepos - glm::vec2(ceil(width / 2.0f), ceil(height / 2.0f))) * (float)tilesize + glm::vec2(tilesize * 0.5f);
}

void Tilemap::draw() {
	tiledrawer.use();
	// make sure to set MVP before drawing
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
	mView = uView;
	tiledrawer["uView"] = glm::transpose(uView);
}

void Tilemap::setModelMatrix(glm::mat3 &uModel) {
	mModel = uModel;
	tiledrawer["uModel"] = glm::transpose(uModel);
}


void Tilemap::clearCache() {
	tileVertexData.clear();
	tileTexCoordData.clear();
	blendTexCoordData.clear();
	overlapTexCoordData.clear();
	tileVertexData.resize(1);
	tileTexCoordData.resize(1);
	blendTexCoordData.resize(1);
	overlapTexCoordData.resize(1);
	fprintf(stdout, "cleared tilemap\n");
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

void Tilemap::setTileTexCoords(std::vector<glm::vec2> &target, int tx, int ty, glm::vec2 s, float angle) {
	float pixel = 0.1 / getTilesetSizeInPixels(); // TODO: fix texcoord errors on zoom
	float n_tilesize = 1.0 / (double)tilesetSize;
	int i = tileToIndex(tx, ty, width, height);
	
	glm::mat2 rotation(
		cos(angle), sin(angle),
		-sin(angle), cos(angle));
	
	glm::vec2 nw = glm::vec2(0.0f + pixel,        0.0f + pixel);
	glm::vec2 ne = glm::vec2(n_tilesize - pixel,  0.0f + pixel);
	glm::vec2 se = glm::vec2(n_tilesize - pixel,  n_tilesize - pixel);
	glm::vec2 sw = glm::vec2(0.0f + pixel,        n_tilesize - pixel);
	glm::vec2 halftile = se * 0.5f;
	
	target.at(i * 6 + 0) = s * n_tilesize + (nw - halftile) * rotation + halftile;
	target.at(i * 6 + 1) = s * n_tilesize + (ne - halftile) * rotation + halftile;
	target.at(i * 6 + 2) = s * n_tilesize + (sw - halftile) * rotation + halftile;
	target.at(i * 6 + 3) = s * n_tilesize + (ne - halftile) * rotation + halftile;
	target.at(i * 6 + 4) = s * n_tilesize + (se - halftile) * rotation + halftile;
	target.at(i * 6 + 5) = s * n_tilesize + (sw - halftile) * rotation + halftile;
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
