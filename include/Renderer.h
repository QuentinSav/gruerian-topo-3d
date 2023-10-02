#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Mesh.h"
#include "MeshDEM.h"
#include "Shader.h"
#include "Light.h"
#include "InputController.h"
#include "Texture.h"

class Renderer {
private:
    // Constants
    const unsigned int m_screen_width = 1200;
    const unsigned int m_screen_height = 900;

    GLFWwindow* m_window; 

    // Objects
    Camera& m_camera;
    InputController& m_input_controller = InputController::getInstance();
    std::vector<Mesh*> m_meshes;
       
    glm::vec3 m_light_position = glm::vec3(50.0f, 1.0f, -40.0f);

public:
    Renderer(Camera& camera);
    ~Renderer();
    void renderLoop();
    void link(Mesh* mesh);
};

#endif // RENDERER_H