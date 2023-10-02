#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>

#include <stdlib.h>
#include <stdio.h>
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
#include "Skybox.h"

int main()
{   
    // INSTANCIATE THE RENDERER
    Camera camera(glm::vec3(0.0f, 0.1f, 0.0f));
    Renderer renderer(camera);

    // SET UP VERTEX DATA - BUFFERS AND CONFIG VERTEX ATTRIBUTES ------------------------------
    // Define vertices
    MeshDEM mesh;
    //mesh.loadVerticesFromTIFF("../../gruerian-topo-3d-data/swissalti.tif");
    //mesh.loadPredefinedVertices();
    mesh.loadVerticesFromTIFF("../../gruerian-topo-3d-data/SantaMonica.jpg");
    glm::mat4 mesh_transform = glm::mat4(1.0f);
    mesh_transform = glm::translate(mesh_transform, glm::vec3(-0.5f, 0.1f, 0.5f));
    mesh.addTransform(mesh_transform);
    mesh.setup();
    renderer.link(&mesh);

    Mesh lightSource;
    lightSource.loadPredefinedVertices();
    glm::mat4 light_source_transform = glm::mat4(1.0f);
    light_source_transform = glm::translate(light_source_transform, glm::vec3(50.0f, 1.0f, -40.0f));
    light_source_transform = glm::scale(light_source_transform, glm::vec3(0.2f, 0.2f, 0.2f));
    lightSource.addTransform(light_source_transform);
    lightSource.setup();
    renderer.link(&lightSource);

    Skybox skybox("../res/textures/skybox/");
    skybox.addTransform(glm::mat4(1.0f));
    skybox.setup();
    renderer.link(&skybox);

    
    // CREATING, COMPILING AND LINKING SHADERS ---------------------------------------------
    // Init file paths
    std::string vertexShaderFilePath = "../res/shaders/vertex_shader.glsl";
    std::string fragmentShaderFilePath = "../res/shaders/fragment_shader.glsl";
    std::string lightFragmentShaderFilePath = "../res/shaders/light_fragment_shader.glsl";
    std::string skyboxVertexShaderFilePath = "../res/shaders/skybox_vertex.glsl";
    std::string skyboxFragmentShaderFilePath = "../res/shaders/skybox_fragment.glsl";
    
    // Create the shader program
    Shader shaderProgram(vertexShaderFilePath, fragmentShaderFilePath);
    Shader shaderLightProgram(vertexShaderFilePath, lightFragmentShaderFilePath);
    Shader shaderSkyboxProgram(skyboxVertexShaderFilePath, skyboxFragmentShaderFilePath);
    
    mesh.link(&shaderProgram);
    lightSource.link(&shaderLightProgram);
    skybox.link(&shaderSkyboxProgram);

    // GENERATING A TEXTURE -----------------------------------------------------------------------
    // Bind, load and generate the texture    
    //Texture texture1(0);
    //texture1.generate("../res/textures/wall.jpg");
    //Texture texture2(1);
    //texture2.generate("../res/textures/awesomeface.png");
    
    //mesh.link(&texture1);
    //mesh.link(&texture2);

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






