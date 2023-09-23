#include "InputControler.h"
#include "Camera.h"
    
InputControler::InputControler(GLFWwindow* window, Camera& camera) : 
    m_window(window), 
    m_camera(camera) 
{}

void InputControler::process_keyboard_input(float frame_delta_time) {
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
    
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.process_keyboard(FORWARD, frame_delta_time);
    
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.process_keyboard(BACKWARD, frame_delta_time);
    
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera.process_keyboard(LEFT, frame_delta_time);
    
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera.process_keyboard(RIGHT, frame_delta_time);
}
