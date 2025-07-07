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
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_CULL_FACE);
    if (dl != 0) {
        for (int i = 0; i < shapes.size(); i++) {
            glCallList(dl + i);
        }
    }
    else {
        compileLists();
    }
    glPopAttrib();
}

void Model::renderX(float x, float y, float z, float xRot, float yRot, float zRot, float xScale, float yScale, float zScale) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, z);
    glRotatef(zRot, 0, 0, 1);
    glRotatef(xRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);
    glScalef(xScale, yScale, zScale);
    render();
}

void Model::compileLists()
{
    dl = glGenLists(shapes.size());
    for (int i = 0; i < shapes.size(); i++) {
        glNewList(dl + i, GL_COMPILE_AND_EXECUTE);
        glBegin(GL_TRIANGLES);
        int offset = 0;
        int lastMat = -1;
        for (int j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++) {
            int mati = shapes[i].mesh.material_ids[j];
            if (mati >= 0) {
                tinyobj::material_t mat = materials[mati];
                if (mati != lastMat) {
                    if (diffuse[mati] != nullptr) {
                        glEnd();
                        diffuse[mati]->set();
                        glBegin(GL_TRIANGLES);
                    }
                    GLfloat* ambient = new GLfloat[4]{ mat.ambient[0], mat.ambient[1], mat.ambient[2], 1 };
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
                    delete[] ambient;
                    GLfloat* diffuse = new GLfloat[4]{ mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1 };
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
                    delete[] diffuse;
                    GLfloat* specular = new GLfloat[4]{ mat.specular[0], mat.specular[1], mat.specular[2], 1 };
                    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
                    delete[] specular;
                    GLfloat* emission = new GLfloat[4]{ mat.emission[0], mat.emission[1], mat.emission[2], 1 };
                    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
                    delete[] emission;
                    glMaterialf(GL_FRONT, GL_SHININESS, mat.shininess);
                    lastMat = mati;
                }
            }
            for (int k = 0; k < shapes[i].mesh.num_face_vertices[j]; k++) {
                tinyobj::index_t id = shapes[i].mesh.indices[offset + k];

                if (id.normal_index >= 0) {
                    glNormal3f(attrib.normals[3 * size_t(id.normal_index)], attrib.normals[3 * size_t(id.normal_index) + 1], attrib.normals[3 * size_t(id.normal_index) + 2]);
                }

                if (id.texcoord_index >= 0) {
                    glTexCoord2f(attrib.texcoords[2 * size_t(id.texcoord_index)], attrib.texcoords[2 * size_t(id.texcoord_index) + 1]);
                }

                glVertex3f(attrib.vertices[3 * size_t(id.vertex_index)], attrib.vertices[3 * size_t(id.vertex_index) + 1], attrib.vertices[3 * size_t(id.vertex_index) + 2]);
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