#pragma once

using namespace std;

#include <unordered_map>
#include <vector>

#include "tiny_obj_loader.h"
#include "Util.h"
#include "Texture.h"

class Model {
public:
	Model(string filename);
	~Model();
	void render();
private:
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	vector<shared_ptr<Texture>> diffuse;
	//vector<shared_ptr<Texture>> normal;
};

class ModelManager {
public:
	static ModelManager* getInstance();
	shared_ptr<Model> getModel(string name);
	ModelManager();
private:
	static Model loadModel(string name);
	MapCache<string, Model> map;
	static ModelManager* instance;
};