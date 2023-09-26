#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

class InputController {
public:
    
    static InputController& getInstance();
    void initialization(GLFWwindow* window, Camera* camera);

    void framebufferSizeCallback(int width, int height);
    static void framebufferSizeCallbackHandle(GLFWwindow* window, int width, int height);
    
    void cursorPosCallback(double pos_x_in, double pos_y_in);
    static void cursorPosCallbackHandle(GLFWwindow* window, double pos_x_in, double pos_y_in);
    
    void cursorScrollCallback(double offset_x, double offset_y);
    static void cursorScrollCallbackHandle(GLFWwindow* window, double offset_x, double offset_y);

    void processKeyboard(float frame_delta_time);
    
private:
    InputController();
    InputController(const InputController&) = delete; // prevent copy constructor
    InputController& operator=(const InputController&) = delete; // prevent move constructor
    ~InputController();

    GLFWwindow* m_window;
    Camera* m_camera;
};

#endif // INPUTCONTROLLER_H
