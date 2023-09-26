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

#include <Shader.h>
#include <Texture.h>
#include <Camera.h>
#include <InputController.h>
#include <DigitalElevationModel.h>
#include "Renderer.h"


// Timing
float frame_delta_time = 0.0f;	
float last_frame_time = 0.0f;
float current_frame_time = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{   

    Renderer renderer();


    // CREATING, COMPILING AND LINKING SHADERS ---------------------------------------------
    // Init file paths
    std::string vertexShaderFilePath = "../res/shaders/vertex_shader.glsl";
    std::string fragmentShaderFilePath = "../res/shaders/fragment_shader.glsl";
    std::string lightvertexShaderFilePath = "../res/shaders/light_vertex_shader.glsl";
    std::string lightfragmentShaderFilePath = "../res/shaders/light_fragment_shader.glsl";

    // Create the shader program
    Shader lightingShaderProgram(lightvertexShaderFilePath, lightfragmentShaderFilePath);
    Shader shaderProgram(vertexShaderFilePath, fragmentShaderFilePath);

    // SET UP VERTEX DATA - BUFFERS AND CONFIG VERTEX ATTRIBUTES ------------------------------
    // Define vertices

    DigitalElevationModel landscape;
    //landscape.load_vertices_from_TIFF("../../gruerian-topo-3d-data/swissalti.tif");
    //landscape.compute_indexes();
    landscape.load_predefined_vertices();
    landscape.bind();


    // GENERATING A TEXTURE -----------------------------------------------------------------------
    // Bind, load and generate the texture    
    Texture texture1(0);
    texture1.generate("../res/textures/wall.jpg");
    Texture texture2(1);
    texture2.generate("../res/textures/awesomeface.png");
    
    shaderProgram.use();
    shaderProgram.set_uniform("texture1", 0);
    shaderProgram.set_uniform("texture2", 1);


    // Lighting 
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, landscape.m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, landscape.m_EBO);
    // set the vertex attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //load_tiff_function();
    // uncomment to print how many vertex attributes are supported
    //int nrAttributes;
    //glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    //std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // Uncomment to get only the lines
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    

    // RENDERING --------------------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        // Frame timing
        current_frame_time = static_cast<float>(glfwGetTime());
        frame_delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        // input
        input_controller.processKeyboard(frame_delta_time);

        // rendering commands here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShaderProgram.use();
        lightingShaderProgram.set_uniform("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShaderProgram.set_uniform("lightColor", 1.0f, 1.0f, 1.0f);

        // Bind the textures
        texture1.bind();
        texture2.bind();        

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        lightingShaderProgram.set_uniform("projection", projection);
        lightingShaderProgram.set_uniform("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShaderProgram.set_uniform("model", model);

        landscape.bind_vertex_array();
        landscape.draw();
        
        // also draw the lamp object
        shaderProgram.use();
        shaderProgram.set_uniform("projection", projection);
        shaderProgram.set_uniform("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        shaderProgram.set_uniform("model", model);

        //glBindVertexArray(lightCubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);  
        //landscape.draw();
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    glfwTerminate();
    return 0;
}






