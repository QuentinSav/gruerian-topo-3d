#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "InputController.h"
#include "MeshDEM.h"
#include "Renderer.h"
#include "Mesh.h"

int main()
{   
    // INSTANCIATE THE RENDERER
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    Renderer renderer(camera);

    // SET UP VERTEX DATA - BUFFERS AND CONFIG VERTEX ATTRIBUTES ------------------------------
    // Define vertices
    Mesh mesh;
    mesh.loadPredefinedVertices();
    mesh.addTransform(glm::mat4(1.0f));
    mesh.setup();
    renderer.link(&mesh);

    Mesh lightSource;
    lightSource.loadPredefinedVertices();
    glm::mat4 lightSourceTransform = glm::mat4(1.0f);
    lightSourceTransform = glm::translate(lightSourceTransform, glm::vec3(1.2f, 1.0f, 2.0f));
    lightSourceTransform = glm::scale(lightSourceTransform, glm::vec3(0.2f, 0.2f, 0.2f));
    lightSource.addTransform(lightSourceTransform);
    lightSource.setup();
    renderer.link(&lightSource);
    
    
    // CREATING, COMPILING AND LINKING SHADERS ---------------------------------------------
    // Init file paths
    std::string vertexShaderFilePath = "../res/shaders/vertex_shader.glsl";
    std::string fragmentShaderFilePath = "../res/shaders/fragment_shader.glsl";
    std::string lightFragmentShaderFilePath = "../res/shaders/light_fragment_shader.glsl";

    // Create the shader program
    Shader shaderProgram(vertexShaderFilePath, fragmentShaderFilePath);
    Shader shaderLightProgram(vertexShaderFilePath, lightFragmentShaderFilePath);
    
    mesh.link(&shaderProgram);
    lightSource.link(&shaderLightProgram);

    // GENERATING A TEXTURE -----------------------------------------------------------------------
    // Bind, load and generate the texture    
    Texture texture1(0);
    texture1.generate("../res/textures/wall.jpg");
    Texture texture2(1);
    texture2.generate("../res/textures/awesomeface.png");
    
    mesh.link(&texture1);
    mesh.link(&texture2);

    //shaderProgram.use();
    //shaderProgram.set_uniform("texture1", 0);
    //shaderProgram.set_uniform("texture2", 1);


    // Uncomment to get only the lines
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // RENDERING --------------------------------------------------------------
    renderer.renderLoop(); 

    glfwTerminate();
    return 0;
}






