#ifndef GLWRAPPER_TEXTURE_HPP
#define GLWRAPPER_TEXTURE_HPP

#include <string>
#include <GL/gl3w.h>
#include <stb/stb_image.h>

class Texture {
	GLuint texture;
	
	int width, height;
	int channels;
	
public:
	
	bool load(std::string path);
	void destroy();
	
	void set(GLenum pname, GLfloat param);
	
	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
	inline int getChannels() const { return channels; }
	
	void bind(bool active = true);
	void active(int unit);
	
	void loadTextureFromMemory(GLubyte *bitmap);
	void setTextureData(int width, int height, int channels);
	
	operator GLuint() const { return texture; }
};

#endif
