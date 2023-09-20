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

#include <Shader.h>
#include <Texture.h>
#include <Camera.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double pos_x_in, double pos_y_in);
void scroll_callback(GLFWwindow* window, double offset_x, double offset_y);
void processInput(GLFWwindow *window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Timing
float frame_delta_time = 0.0f;	// time between current frame and last frame
float last_frame_time = 0.0f;
float current_frame_time = 0.0f;

// Texture control
float mixRatio = 0.2f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


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
    std::string fragment2ShaderFilePath = "../res/shaders/fragment_shader_2.glsl";

    // Create the shader program
    Shader shaderProgram(vertexShaderFilePath, fragmentShaderFilePath);

    // SET UP VERTEX DATA - BUFFERS AND CONFIG VERTEX ATTRIBUTES ------------------------------
    // Define vertices
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
 
/*    unsigned int indices[] = {  // note that we start from 0!
         0,  1,  2,   // first triangle
         3,  4,  5,   // second triangle
         6,  7,  8,
         9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };
*/
    glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

    // Generate a buffer with unique ID VBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);  
    glGenBuffers(1, &EBO); 
    
    //Bind VAO first
    glBindVertexArray(VAO);
    //Bind the created buffer to the GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    // Copy the vertex data into buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Positions attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Colors attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0); 


    // GENERATING A TEXTURE -----------------------------------------------------------------------
    // Bind, load and generate the texture    
    Texture texture1(0);
    texture1.generate("../res/textures/wall.jpg");
    Texture texture2(1);
    texture2.generate("../res/textures/awesomeface.png");
    
    shaderProgram.use();
    shaderProgram.set_uniform("texture1", 0);
    shaderProgram.set_uniform("texture2", 1);


    // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    // -----------------------------------------------------------------------------------------------------------

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
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind the textures
        texture1.bind();
        texture2.bind();        

        shaderProgram.use();
        shaderProgram.set_uniform("mixRatio", mixRatio);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shaderProgram.set_uniform("projection", projection);

        // Coordinate systems -----------------------------------------------------------------------------
        // camera/view transformation
        shaderProgram.set_uniform("view", camera.get_view_matrix());        

        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderProgram.set_uniform("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);    
        }

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        (mixRatio += 0.01f) ? mixRatio += 0.01f : mixRatio = 1.0f;
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        (mixRatio -= 0.01f) ? mixRatio -= 0.01f : mixRatio = 1.0f;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(FORWARD, frame_delta_time);
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(BACKWARD, frame_delta_time);
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(LEFT, frame_delta_time);
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(RIGHT, frame_delta_time);

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