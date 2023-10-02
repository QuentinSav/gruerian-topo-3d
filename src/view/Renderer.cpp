#include "Renderer.h"

Renderer::Renderer(Camera& camera) :
    m_camera(camera)
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
    m_window = glfwCreateWindow(m_screen_width, m_screen_height, "Gruerian Topo 3D", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        //return -1;
    }

    m_input_controller.initialization(m_window, &m_camera);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1;
    }

    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer()
{
    for (Mesh* mesh: m_meshes)
    {
        glDeleteVertexArrays(1, &mesh->m_VAO);
        glDeleteBuffers(1, &mesh->m_VBO);
    }
}

void Renderer::link(Mesh* mesh)
{
    m_meshes.push_back(mesh);
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

        // Inputs
        m_input_controller.processKeyboard(frame_delta_time);

        // Rendering commands here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Projection transform
        glm::mat4 projection_transform = glm::perspective(glm::radians(m_camera.getZoom()), (float)m_screen_width / (float)m_screen_height, 0.1f, 100.0f);

        for (Mesh* mesh: m_meshes)
        {
            for (glm::mat4 model_transform: mesh->getModelTransforms())
            {
                for (Shader* shader: mesh->getLinkedShaders())
                {
                    shader->use();
                    
                    // Set transformations
                    shader->set_uniform("projection", projection_transform);
                    shader->set_uniform("model", model_transform);
                    shader->set_uniform("view", m_camera.getViewMatrix());
                    shader->set_uniform("view_cubemap", m_camera.getViewMatrix());
                    //shader->set_uniform("view_cubemap", glm::mat4(glm::mat3(m_camera.getViewMatrix()))); // Removed translation

                    // Set lighting
                    shader->set_uniform("objectColor", glm::vec3(0.2f, 0.5f, 1.0f));
                    shader->set_uniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
                    shader->set_uniform("lightPos", m_light_position);
                    shader->set_uniform("viewPos", m_camera.getPosition());
                    
                    for (Texture* texture: mesh->getLinkedTextures())
                    {
                        //texture->bind();
                        shader->set_uniform(("texture"+std::to_string(texture->getId())), texture->getId());
                    }
                }
                
                mesh->bindVertexArray();
                mesh->draw();
                
            }
        }

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}