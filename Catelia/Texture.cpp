#include "Texture.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <filesystem>

TextureManager* TextureManager::instance;

Texture::Texture(string filename)
{
	printf("Load texture %s\n", filename.c_str());
	stbi_set_flip_vertically_on_load(true);
	tex = stbi_load(filename.c_str(), &w, &h, &n, 4);
	if (!tex) {
		printf("Failed to load texture %s\n", filename.c_str());
	}
	glGenTextures(1, &glTex);
	printf("%s: %d\n", filename.c_str(), glTex);
	glBindTexture(GL_TEXTURE_2D, glTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
}

Texture::~Texture()
{
	printf("Texture destroyed.\n");
	// Uncomment this once the Texture stops being destroyed immediatly 
	//glDeleteTextures(1, &glTex);
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
	map = MapCache<string, Texture>();
	map.load = loadTexture;
	instance = this;
}

Texture TextureManager::loadTexture(string name)
{
	return Texture(name); // the texture manager is not directly called by the end user, and as such takes paths rather than names, as this is more convenient for everything that WILL be calling it
}
