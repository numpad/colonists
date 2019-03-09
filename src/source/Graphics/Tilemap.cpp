#include "Graphics/Tilemap.hpp"

static inline size_t tileToIndex(int tx, int ty, int w, int h) {
	(void)h; // TODO: check if inside bounds
	return tx + ty * w;
}

Tilemap::Tilemap(int w, int h):
	width(w), height(h),
	tiledrawer("res/glsl/tilemap/drawsimple.vert.glsl", "res/glsl/tilemap/drawsimple.frag.glsl"),
	tileVertexData((w+1) * (h+1) * 2),
	tileIndexData(w * h * 6),
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
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	tileset->active(0);
	tiledrawer.use();
	glBindVertexArray(tilesVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tilesIndices);
	
	glDrawElements(GL_TRIANGLES, width * height * 6, GL_UNSIGNED_INT, (void *)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	
}


/// PRIVATE

void Tilemap::setTileTexCoords(int tx, int ty, glm::vec2 s) {
	(void)tileTexCoordData;
	
	const glm::vec2 tRight = glm::vec2((float)tilesetTilesize, 0.0f) / 960.0f;
	const glm::vec2 tDown  = glm::vec2(0.0f, (float)tilesetTilesize) / 960.0f;
	
	int i = tileToIndex(tx, ty, width, height);
	
	tileTexCoordData.at(i * 6 + 0) = glm::vec2(0.0f, 0.0f);
	tileTexCoordData.at(i * 6 + 1) = glm::vec2(0.1f, 0.0f);
	tileTexCoordData.at(i * 6 + 2) = glm::vec2(0.0f, 0.1f);
	tileTexCoordData.at(i * 6 + 3) = glm::vec2(0.1f, 0.0f);
	tileTexCoordData.at(i * 6 + 4) = glm::vec2(0.1f, 0.1f);
	tileTexCoordData.at(i * 6 + 5) = glm::vec2(0.0f, 0.1f);
}

void Tilemap::loadTileVertices() {
	glGenVertexArrays(1, &tilesVAO);
	glGenBuffers(1, &tilesVBO);
	glGenBuffers(1, &tilesIndices);
	glGenBuffers(1, &tilesTexCoords);
	
	this->generateTileVertices();
	
	glBindVertexArray(tilesVAO);
		glBindBuffer(GL_ARRAY_BUFFER, tilesVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tilesIndices);
		
		glBufferData(GL_ARRAY_BUFFER,
			tileVertexData.size() * sizeof(GLfloat),
			&tileVertexData[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			tileIndexData.size() * sizeof(GLuint),
			&tileIndexData[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
		
		
		glBindBuffer(GL_ARRAY_BUFFER, tilesTexCoords);
		
		glBufferData(GL_ARRAY_BUFFER,
			tileTexCoordData.size() * 2 * sizeof(GLfloat),
			(GLfloat *)&tileTexCoordData[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static void setSingleTileIndices(std::vector<GLuint> &indices, int x, int y, int w, int h) {
	int i = tileToIndex(x, y, w, h);
	int bi = tileToIndex(x, y, w + 1, h + 1);
	indices.at(i * 6 + 0) = bi;
	indices.at(i * 6 + 1) = bi + 1;
	indices.at(i * 6 + 2) = bi + w+1;
	indices.at(i * 6 + 3) = bi + 1;
	indices.at(i * 6 + 4) = bi + 1 + w+1;
	indices.at(i * 6 + 5) = bi + w+1;
	
}

void Tilemap::generateTileVertices() {
	
	// generate a grid for the tiles vertices
	for (int y = 0; y < height + 1; ++y) {
		for (int x = 0; x < width + 1; ++x) {
			glm::vec2 v = glm::vec2(x, y) * (float)tilesize;
			tileVertexData.at(tileToIndex(x, y, width + 1, height + 1) * 2 + 0) = (float)x * (float)tilesize;
			tileVertexData.at(tileToIndex(x, y, width + 1, height + 1) * 2 + 1) = (float)y * (float)tilesize;
			
			if (x < width && y < height) {
				setSingleTileIndices(tileIndexData, x, y, width, height);
				setTileTexCoords(x, y, glm::vec2(0.0f));
			}
		}
	}
}

void Tilemap::destroyTileVertices() {
	glDeleteVertexArrays(1, &tilesVAO);
	glDeleteBuffers(1, &tilesVBO);
	glDeleteBuffers(1, &tilesIndices);
	glDeleteBuffers(1, &tilesTexCoords);
}
