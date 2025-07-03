#include "Texture.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <filesystem>

TextureManager* TextureManager::instance;

Texture::Texture(string filename)
{
	//printf("Load texture %s\n", filename.c_str());
	stbi_set_flip_vertically_on_load(true);
	tex = stbi_load(filename.c_str(), &w, &h, &n, 4);
	if (!tex) {
		printf("Failed to load texture %s\n", filename.c_str());
	}
	compile();
}

/*Texture::Texture(const Texture& o)
{
	w = o.w;
	h = o.h;
	n = o.n;
	tex = (unsigned char*)malloc((w * h) * 4);
	memcpy(tex, o.tex, (w * h) * 4);
	glGenTextures(1, &glTex);
	glBindTexture(GL_TEXTURE_2D, glTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
}*/

Texture::~Texture()
{
	printf("Texture destroyed.\n");
	// Uncomment this once the Texture stops being destroyed immediatly 
	glDeleteTextures(1, &glTex);
	stbi_image_free(tex);
}

void Texture::set()
{
	glBindTexture(GL_TEXTURE_2D, glTex);
}

void Texture::compile() {
	glGenTextures(1, &glTex);
	glBindTexture(GL_TEXTURE_2D, glTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
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

shared_ptr<Texture> TextureManager::loadTexture(string name)
{
	return make_shared<Texture>(name); // the texture manager is not directly called by the end user, and as such takes paths rather than names, as this is more convenient for everything that WILL be calling it
}
