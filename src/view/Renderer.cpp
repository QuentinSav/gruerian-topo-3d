#include "Renderer.h"

Renderer::Renderer()
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
    m_window = glfwCreateWindow(m_screen_width, m_screen_width, "Gruerian Topo 3D", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        //return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1;
    }

    glEnable(GL_DEPTH_TEST);

    m_input_controller.initialization(m_window, &m_camera);
}

Renderer::~Renderer()
{
    for (Model* model: m_models)
    {
        glDeleteVertexArrays(1, &model->m_VAO);
        glDeleteBuffers(1, &model->m_VBO);
    }
}

void Renderer::renderLoop()
{   
    // Timing
    float frame_delta_time = 0.0f;	
    float last_frame_time = 0.0f;
    float current_frame_time = 0.0f;
    
    while(!glfwWindowShouldClose(m_window))
    {
        // Frame timing
        current_frame_time = static_cast<float>(glfwGetTime());
        frame_delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        // input
        m_input_controller.processKeyboard(frame_delta_time);

        // rendering commands here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Model* model: m_models)
        {
            for (Shader* shader: model->m_shaders)
            {
                shader->use();
                
                for (shaderUniform: shader->uniforms)
                {
                    shader->set_uniform(shaderUniform, shaderUniformValue);
                }
            }

            for (Textures* texture: model->m_textures)
            {
                bindTexture();
            }

            model->bind();
        }

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}