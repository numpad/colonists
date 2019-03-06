#ifndef GRAPHICS_TILEMAP_HPP
#define GRAPHICS_TILEMAP_HPP

#include <GL/gl3w.h>

class Tilemap {
	
	int width, height;
	
public:
	
	Tilemap(int width, int height);
	~Tilemap();
	
	void generate();
	
	void draw();
};

#endif
