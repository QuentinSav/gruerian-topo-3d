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
#include <InputControler.h>
#include <DigitalElevationModel.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double pos_x_in, double pos_y_in);
void scroll_callback(GLFWwindow* window, double offset_x, double offset_y);
//void processInput(GLFWwindow *window);

// Settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// Timing
float frame_delta_time = 0.0f;	
float last_frame_time = 0.0f;
float current_frame_time = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{   

    // GLFW INIT AND CONF -------------------------------------------------------------------
    // Init windows and parameters
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW WINDOW CREATION -----------------------------------------------------------------
    // Create window object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gruerian Topo 3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

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
    //molesonMap.load_vertices_from_TIFF("../../gruerian-topo-3d-data/swissalti.tif");
    //molesonMap.compute_indexes();
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

    InputControler input_controler(window, camera);

    // RENDERING --------------------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        // Frame timing
        current_frame_time = static_cast<float>(glfwGetTime());
        frame_delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        // input
        input_controler.process_keyboard_input(frame_delta_time);

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
        glm::mat4 view = camera.get_view_matrix();
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  


void mouse_callback(GLFWwindow* window, double pos_x_in, double pos_y_in)
{
    float pos_x = static_cast<float>(pos_x_in);
    float pos_y = static_cast<float>(pos_y_in);
    
    static float pos_x_last = pos_x;
    static float pos_y_last = pos_y;

    float offset_x = pos_x - pos_x_last;
    float offset_y = pos_y_last - pos_y; 

    pos_x_last = pos_x;
    pos_y_last = pos_y;

    camera.process_mouse_movement(offset_x, offset_y);
}  

void scroll_callback(GLFWwindow* window, double offset_x, double offset_y)
{
    float offset = static_cast<float>(offset_y);
    camera.process_mouse_scroll(offset);
}

