#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Model.h"
#include "DigitalElevationModel.h"
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
    std::vector<Model*> m_models;
    std::vector<Shader*> m_shaders;
    std::vector<Texture*> m_textures;
    //std::vector<Light*> m_lights;   

public:
    Renderer();
    ~Renderer();
    void renderLoop();


};

#endif // RENDERER_H