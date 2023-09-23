#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch):
        m_position(position),
        m_up(up),
        m_yaw(yaw),
        m_pitch(pitch),
        m_world_up(up),
        m_front(FRONT),
        m_movement_speed(SPEED),
        m_mouse_sensitivity(SENSITIVITY),
        m_zoom(ZOOM) 
{
    update_camera_vectors();
}

void Camera::process_keyboard(Camera_Movement direction, const float& frame_delta_time)
{
    float velocity = frame_delta_time * m_movement_speed;
    if (direction == FORWARD)
        m_position += velocity * m_front;
    if (direction == BACKWARD)
        m_position -= velocity * m_front;
    if (direction == LEFT)
        m_position -= velocity * m_right;
    if (direction == RIGHT)
        m_position += velocity * m_right;
}

void Camera::process_mouse_movement(float& offset_x, float& offset_y)
{
    m_yaw   += (offset_x * m_mouse_sensitivity);
    m_pitch += (offset_y * m_mouse_sensitivity);

    (m_pitch > 89.0f) ? m_pitch = 89.0f : m_pitch = m_pitch;
    (m_pitch < -89.0f) ? m_pitch = -89.0f : m_pitch = m_pitch;

    update_camera_vectors();
}

void Camera::process_mouse_scroll(float& offset_y)
{
    m_zoom -= offset_y;
        if (m_zoom < 1.0f)
            m_zoom = 1.0f;
        if (m_zoom > 45.0f)
            m_zoom = 45.0f;
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::update_camera_vectors()
{
    // calculate the new front direction vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_world_up));  
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}