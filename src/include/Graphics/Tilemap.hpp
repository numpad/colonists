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
	
	std::vector<GLuint> tileIDs;
	
	std::vector<glm::vec2> tileVertexData, tileTexCoordData,
		blendTexCoordData, overlapTexCoordData;
	
	GLuint tilesVAO, tilesVBO, tilesTexCoords,
		blendTexCoords, overlapTexCoords;
	
	void loadTileVertices();
	void generateTileVertices();
	void destroyTileVertices();
	
	void setTileIndices(int x, int y);
	void setTileVertices(int tx, int ty);
	void setTileTexCoords(std::vector<glm::vec2> &target, int tx, int ty, glm::vec2 s, float angle = 0.0f);
	
	int getTilesetSizeInPixels();
	
public:
	
	Tilemap(int width, int height);
	~Tilemap();
	
	void setProjectionMatrix(glm::mat3 &uProj);
	void setViewMatrix(glm::mat3 &uView);
	void setModelMatrix(glm::mat3 &uModel);
	
	void getSize(int *cols, int *rows);
	int getWidth();
	int getHeight();
	
	int getTileID(int x, int y);
	
	void setTileID(int x, int y, int id);
	void setBlendTileID(int x, int y, int blend_id, int overlap_id, float blend_rotation = 0.0f);
	void updateTileIDs(); // upload changes from set*TileID() to gpu
	
	void loadTileset(std::string path);

	glm::ivec2 mapWorldToTile(glm::vec2 pos);

	void draw();
};

#endif
