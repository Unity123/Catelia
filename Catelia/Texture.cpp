#include "Texture.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <filesystem>

TextureManager* TextureManager::instance;

Texture::Texture(string filename)
{
	tex = stbi_load(filename.c_str(), &w, &h, &n, 4);
	glGenTextures(1, &glTex);
	glBindTexture(GL_TEXTURE_2D, glTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
}

Texture::~Texture()
{
	glDeleteTextures(1, &glTex);
	stbi_image_free(tex);
}

void Texture::set()
{
	glBindTexture(GL_TEXTURE_2D, glTex);
}

TextureManager* TextureManager::getInstance()
{
	return instance;
}

shared_ptr<Texture> TextureManager::getTexture(string name)
{
	return map.get(name);
}

TextureManager::TextureManager()
{
	map.load = loadTexture;
	instance = this;
}

Texture TextureManager::loadTexture(string name)
{
	return Texture(name); // the texture manager is not directly called by the end user, and as such takes paths rather than names, as this is more convenient for everything that WILL be calling it
}
