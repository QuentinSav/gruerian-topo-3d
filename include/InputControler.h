#ifndef INPUTCONTROLER_H
#define INPUTCONTROLER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

class InputControler {
public:

    InputControler(GLFWwindow* window, Camera& camera);
    void process_keyboard_input(float frame_delta_time);

private:
    GLFWwindow* m_window;
    Camera& m_camera;
};

#endif // INPUTCONTROLER_H