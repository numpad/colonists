#ifndef GRAPHICS_TILEMAP_HPP
#define GRAPHICS_TILEMAP_HPP

#include <string>
#include <vector>
#include <math.h>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <FastNoise/FastNoise.h>
#include "GLWrapper/sgl_shader.hpp"
#include "GLWrapper/Texture.hpp"
#include "System/Window.hpp"

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
	
	glm::mat3 mView, mModel;
	
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
	
	void translate(glm::vec2 offset);
	void scale(float s);
	
	int getTileID(int x, int y);
	
	void setTileID(int x, int y, int id);
	void setBlendTileID(int x, int y, int blend_id, int overlap_id, float blend_rotation = 0.0f);
	void updateTileIDs(); // upload changes from set*TileID() to gpu
	
	void loadTileset(std::string path);

	glm::vec2 mapLocalToWorldCoords(Window &window, glm::vec2 local);
	glm::ivec2 mapWorldToTileCoords(glm::vec2 worldpos);
	
	glm::vec2 mapTileToWorldCoords(glm::ivec2 tilepos);
	
	void clearCache();
	
	void draw();
};

#endif
