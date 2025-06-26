#include "Model.h"
#include <filesystem>

using namespace std;

shared_ptr<Model> ModelManager::getModel(string name)
{
    return map.get(name);
}

ModelManager::ModelManager()
{
    map.load = loadModel;
}

Model ModelManager::loadModel(string name)
{
    return Model(filesystem::current_path().append("/model/").append(name).append(".obj").generic_string());
}

Model::Model(string filename)
{
}
