#include "GLWrapper/Texture.hpp"

static void read_bitmap_to_texture(Texture *texture, std::string path) {
	int width, height, channels;
	stbi_set_flip_vertically_on_load(0);
	GLubyte *bitmap = stbi_load(path.c_str(), &width, &height, &channels, 0);
	
	texture->setTextureData(width, height, channels);
	texture->loadTextureFromMemory(bitmap);
	
	stbi_image_free(bitmap);
}

static GLenum channelsToFormat(int channels) {
	switch (channels) {
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		default: return GL_RGBA;
	};
}

void Texture::setTextureData(int w, int h, int ch) {
	this->width = w;
	this->height = h;
	this->channels = ch;
}

void Texture::updateTextureFromMemory(GLubyte *bitmap) {
	set(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	set(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	bind(true);
	
	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0, 0,
		this->width, this->height,
		channelsToFormat(this->channels),
		GL_UNSIGNED_BYTE,
		bitmap
	);
	
	if (generateMipmapOnLoad) glGenerateMipmap(GL_TEXTURE_2D);
	
	bind(false);
}

void Texture::loadTextureFromMemory(GLubyte *bitmap) {
	glGenTextures(1, &this->texture);
	
	set(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	set(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	bind(true);
	
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		channelsToFormat(this->channels),
		this->width, this->height,
		0,
		channelsToFormat(this->channels),
		GL_UNSIGNED_BYTE,
		bitmap
	);
	
	if (generateMipmapOnLoad) glGenerateMipmap(GL_TEXTURE_2D);
	
	bind(false);
}

bool Texture::load(std::string path) {
	
	read_bitmap_to_texture(this, path);
	
	return true;
}

void Texture::destroy() {
	glDeleteTextures(1, &texture);
}

void Texture::set(GLenum pname, GLfloat param) {
	bind(true);
	
	glTexParameteri(GL_TEXTURE_2D, pname, param);
	
	bind(false);
}

void Texture::bind(bool active) {
	static GLint prev_bind = 0;
	if (active) glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_bind);
	
	glBindTexture(GL_TEXTURE_2D, active ? this->texture : prev_bind);
}

void Texture::active(int unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}
