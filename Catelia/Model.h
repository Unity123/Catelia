#pragma once

using namespace std;

#include <unordered_map>
#include <vector>

#include "tiny_obj_loader.h"
#include "Util.h"

class Model {
public:
	Model(string filename);
	void render();
private:
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
};

class ModelManager {
public:
	shared_ptr<Model> getModel(string name);
	ModelManager();
private:
	static Model loadModel(string name);
	MapCache<string, Model> map;
};