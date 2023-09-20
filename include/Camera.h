// Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default values
const float YAW          = -90.0f;
const float PITCH        =  0.0f;
const float ROLL         =  0.0f;
const float SPEED        =  2.5f;
const float SENSITIVITY  =  0.1f;
const float ZOOM         =  45.0f;
const glm::vec3 POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 UP       = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 FRONT    = glm::vec3(0.0f, 0.0f, -1.0f);

class Camera 
{
public:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_world_up;

    float m_yaw;
    float m_pitch;
    float m_roll;
    
    float m_movement_speed;
    float m_mouse_sensitivity;
    float m_zoom;
    
    Camera(glm::vec3 position = POSITION, glm::vec3 up = UP, float yaw = YAW, float pitch = PITCH);
    
    void process_keyboard(Camera_Movement direction, const float& frame_delta_time);
    void process_mouse_movement(float& offset_x, float& offset_y);
    void process_mouse_scroll(float& offset_y);

    glm::mat4 get_view_matrix();
    
    
private:
    void update_camera_vectors();
};

#endif // CAMERA_H