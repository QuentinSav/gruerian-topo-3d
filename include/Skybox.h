#ifndef SKYBOX_H
#define SKYBOX_H

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
#include "Mesh.h"

struct VertexPosition
{
    float x, y, z;
};

class Skybox : public Mesh {
public:
    Skybox(std::string directory);
    virtual void draw();
    virtual void setup();

protected:
    std::vector<VertexPosition> m_vertices; 
    void loadVertices();

    std::vector<std::string> m_name_faces = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};

};

#endif // SKYBOX_H