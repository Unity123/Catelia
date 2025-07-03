#include "Model.h"
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <filesystem>

using namespace std;

ModelManager* ModelManager::instance;

ModelManager* ModelManager::getInstance()
{
    return instance;
}

shared_ptr<Model> ModelManager::getModel(string name)
{
    shared_ptr<Model> tmp = map.get(name);
    return tmp;
}

ModelManager::ModelManager()
{
    map = MapCache<string, Model>();
    map.load = loadModel;
    instance = this;
}

shared_ptr<Model> ModelManager::loadModel(string name)
{
    return make_shared<Model>((filesystem::current_path() / "model" / name.append(".obj")).generic_string());
}

Model::Model(string filename)
{
    //printf("Loading Model: %s", filename.c_str());
    tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, filename.c_str(), (filesystem::current_path() / "model" / "").generic_string().c_str());
    for (int i = 0; i < materials.size(); i++) {
        if (materials[i].diffuse_texname != "") {
            diffuse.push_back(TextureManager::getInstance()->getTexture((filesystem::current_path() / "model" / materials[i].diffuse_texname).generic_string()));
        }
        else {
            diffuse.push_back(nullptr);
        }
    }
    dl = 0;
}

void Model::render()
{
    if (dl != 0) {
        for (int i = 0; i < shapes.size(); i++) {
            glCallList(dl + i);
        }
    }
    else {
        compileLists();
    }
}

void Model::compileLists()
{
    dl = glGenLists(shapes.size());
    for (int i = 0; i < shapes.size(); i++) {
        glNewList(dl + i, GL_COMPILE_AND_EXECUTE);
        glBegin(GL_TRIANGLES);
        int offset = 0;
        int lastMat = 0;
        for (int j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++) {
            int mati = shapes[i].mesh.material_ids[j];
            if (mati >= 0) {
                tinyobj::material_t mat = materials[mati];
                if (diffuse[mati] != nullptr && mati != lastMat) {
                    glEnd();
                    diffuse[mati]->set();
                    glBegin(GL_TRIANGLES);
                    lastMat = mati;
                }
            }
            for (int k = 0; k < shapes[i].mesh.num_face_vertices[j]; k++) {
                tinyobj::index_t id = shapes[i].mesh.indices[offset + k];

                glVertex3f(attrib.vertices[3 * size_t(id.vertex_index)], attrib.vertices[3 * size_t(id.vertex_index) + 1], attrib.vertices[3 * size_t(id.vertex_index) + 2]);

                if (id.normal_index >= 0) {
                    glNormal3f(attrib.normals[3 * size_t(id.normal_index)], attrib.normals[3 * size_t(id.normal_index) + 1], -attrib.normals[3 * size_t(id.normal_index) + 2]);
                }

                if (id.texcoord_index >= 0) {
                    glTexCoord2f(attrib.texcoords[2 * size_t(id.texcoord_index)], attrib.texcoords[2 * size_t(id.texcoord_index) + 1]);
                }
            }
            offset += shapes[i].mesh.num_face_vertices[j];
        }
        glEnd();
        glEndList();
    }
}

Model::~Model() {
    printf("Model destroyed.\n");
}