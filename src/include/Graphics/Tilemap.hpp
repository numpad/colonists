#ifndef GRAPHICS_TILEMAP_HPP
#define GRAPHICS_TILEMAP_HPP

#include <string>
#include <vector>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <FastNoise/FastNoise.h>
#include "GLWrapper/sgl_shader.hpp"
#include "GLWrapper/Texture.hpp"

class Tilemap {
	
	sgl::shader tiledrawer;
	Texture *tileset = nullptr;
	int width, height; // tile grid size: (width, height)
	int tilesize = 128;
	
	std::vector<GLfloat> tileVertexData;
	std::vector<GLuint> tileIndexData, tileIDs;
	GLuint tilesVAO, tilesVBO, tilesIndices;
	
	void loadTileVertices();
	void generateTileVertices();
	void destroyTileVertices();
public:
	
	Tilemap(int width, int height);
	~Tilemap();
	
	void loadTileset(std::string path);

	void draw(glm::mat3 &uProjection, glm::mat3 &uView);
};

#endif
