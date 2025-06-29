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
    return map.get(name);
}

ModelManager::ModelManager()
{
    map.load = loadModel;
    instance = this;
}

Model ModelManager::loadModel(string name)
{
    return Model((filesystem::current_path() / "model" / name.append(".obj")).generic_string());
}

Model::Model(string filename)
{
    printf(filename.c_str());
    tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, filename.c_str(), (filesystem::current_path() / "model" / "").generic_string().c_str());
    for (int i = 0; i < materials.size(); i++) {
        if (materials[i].diffuse_texname != "") {
            diffuse.push_back(TextureManager::getInstance()->getTexture((filesystem::current_path() / "model" / materials[i].diffuse_texname).generic_string()));
        }
        else {
            diffuse.push_back(nullptr);
        }
    }
}

void Model::render()
{
    for (int i = 0; i < shapes.size(); i++) {
        glBegin(GL_TRIANGLES);
        int offset = 0;
        for (int j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++) {
            int mati = shapes[i].mesh.material_ids[j];
            if (mati >= 0) {
                tinyobj::material_t mat = materials[mati];
                if (diffuse[mati] != nullptr) {
                    diffuse[mati]->set();
                }
            }
            for (int k = 0; k < shapes[i].mesh.num_face_vertices[j]; k++) {
                tinyobj::index_t id = shapes[i].mesh.indices[offset + k];

                glVertex3f(attrib.vertices[3 * size_t(id.vertex_index)], attrib.vertices[3 * size_t(id.vertex_index) + 1], attrib.vertices[3 * size_t(id.vertex_index) + 2]);

                if (id.normal_index >= 0) {
                    glNormal3f(attrib.normals[3 * size_t(id.normal_index)], attrib.normals[3 * size_t(id.normal_index) + 1], attrib.normals[3 * size_t(id.normal_index) + 2]);
                }

                if (id.texcoord_index >= 0) {
                    glTexCoord2f(attrib.texcoords[2 * size_t(id.texcoord_index)], attrib.texcoords[2 * size_t(id.texcoord_index) + 1]);
                }
            }
            offset += shapes[i].mesh.num_face_vertices[j];
        }
        glEnd();
    }
}
