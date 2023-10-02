#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "Shader.h"

struct Vertex
{
    float x, y, z;
    float normal_x, normal_y, normal_z;
};

struct Face 
{
    unsigned int p1, p2, p3;
};


class Mesh {
public:
    Mesh();
    ~Mesh();

    void link(Texture* texture);
    std::vector<Texture*> getLinkedTextures();

    void link(Shader* shader);
    std::vector<Shader*> getLinkedShaders(); 
    
    void addTransform(glm::mat4 m_model_transforms);
    std::vector<glm::mat4> getModelTransforms();

    unsigned int m_size_x, m_size_y;
    unsigned int m_VBO, m_VAO, m_EBO;

    void loadPredefinedVertices();
    virtual void setup();
    void bindVertexArray();
    virtual void draw();

protected:
    std::vector<Vertex> m_vertices;    
    std::vector<Face> m_faces;
    std::vector<Texture*> m_textures;
    std::vector<Shader*> m_shaders;
    std::vector<glm::mat4> m_model_transforms;
};

#endif // MESH_H