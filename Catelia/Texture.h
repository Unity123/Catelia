#pragma once

#include <string>
#include <memory>
#include "Util.h"
using namespace std;

class Texture {
public:
	Texture(string filename);
	~Texture();
	void set();
	int getW();
	int getH();
private:
	unsigned char* tex;
	int w;
	int h;
	int n;
	unsigned int glTex;
};

inline int Texture::getW()
{
	return w;
}

inline int Texture::getH()
{
	return h;
}

class TextureManager {
public:
	static TextureManager* getInstance();
	shared_ptr<Texture> getTexture(string name);
	TextureManager();
private:
	static Texture loadTexture(string name);
	MapCache<string, Texture> map;
	static TextureManager* instance;
};