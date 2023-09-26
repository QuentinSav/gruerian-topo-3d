/**
 * @class InputController
 * @brief This class handles all the user inputs.
 */

#include "InputController.h"

InputController::InputController() :
    m_window(nullptr),
    m_camera(nullptr)
{}

void InputController::initialization(GLFWwindow* window, Camera* camera)
{   
    // Initialize the member parameters
    m_window = window;
    m_camera = camera;

    // Set the GLFW context, bind callbacks, etc
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, &InputController::framebufferSizeCallbackHandle);
    glfwSetCursorPosCallback(m_window, &InputController::cursorPosCallbackHandle);
    glfwSetScrollCallback(m_window, &InputController::cursorScrollCallbackHandle);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

InputController::~InputController()
{   
    // Unbind callbacks
    glfwSetFramebufferSizeCallback(m_window, NULL);
    glfwSetCursorPosCallback(m_window, NULL);
    glfwSetScrollCallback(m_window, NULL);
}

InputController& InputController::getInstance()
{
    static InputController instance;
    return instance;
}

void InputController::cursorPosCallback(double pos_x_in, double pos_y_in)
{
    // Static initialization of the last position
    static float pos_x_last = static_cast<float>(pos_x_in);
    static float pos_y_last = static_cast<float>(pos_y_in);

    // Compute offsets
    float offset_x = static_cast<float>(pos_x_in) - pos_x_last;
    float offset_y = pos_y_last - static_cast<float>(pos_y_in); 

    // Update last positions
    pos_x_last = static_cast<float>(pos_x_in);
    pos_y_last = static_cast<float>(pos_y_in);

    // Update camera
    m_camera->processRotation(offset_x, offset_y);
}  

void InputController::cursorPosCallbackHandle(GLFWwindow* window, double pos_x_in, double pos_y_in)
{
    getInstance().cursorPosCallback(pos_x_in, pos_y_in); 
}

void InputController::cursorScrollCallback(double offset_x, double offset_y)
{
    float offset = static_cast<float>(offset_y);
    m_camera->processZoom(offset);
}

void InputController::cursorScrollCallbackHandle(GLFWwindow* window, double offset_x, double offset_y)
{
    getInstance().cursorScrollCallback(offset_x, offset_y); 
}

void InputController::framebufferSizeCallback(int width, int height)
{
    glViewport(0, 0, width, height);
}

void InputController::framebufferSizeCallbackHandle(GLFWwindow* window, int width, int height)
{
    getInstance().framebufferSizeCallback(width, height); 
}

void InputController::processKeyboard(float frame_delta_time) 
{
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
    
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->processTranslation(FORWARD, frame_delta_time);
    
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->processTranslation(BACKWARD, frame_delta_time);
    
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->processTranslation(LEFT, frame_delta_time);
    
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->processTranslation(RIGHT, frame_delta_time);
}
