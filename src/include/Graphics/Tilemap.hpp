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
	int tilesize = 64;
	int tilesetTilesize = 64, tilesetSize = 10; // pixels/block, blocks_(row,col)/image
	
	std::vector<glm::vec2> tileVertexData, tileTexCoordData;
	std::vector<GLuint> tileIDs;
	GLuint tilesVAO, tilesVBO, tilesTexCoords;
	
	void loadTileVertices();
	void generateTileVertices();
	void destroyTileVertices();
	
	void setTileIndices(int x, int y);
	void setTileVertices(int tx, int ty);
	void setTileTexCoords(int tx, int ty, glm::vec2 s);
	
	int getTilesetSizeInPixels();
	
public:
	
	Tilemap(int width, int height);
	~Tilemap();
	
	void setProjectionMatrix(glm::mat3 &uProj);
	void setViewMatrix(glm::mat3 &uView);
	void setModelMatrix(glm::mat3 &uModel);
	
	void getSize(int *cols, int *rows);
	
	void setTileID(int x, int y, int id, bool is_last = true);
	
	void loadTileset(std::string path);

	void draw();
};

#endif
