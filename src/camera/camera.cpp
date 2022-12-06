#include <stdexcept>
#include "camera.h"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera() {}

Camera::Camera(int width, int height, const SceneCameraData &cameraData, float nearPlane, float farPlane) {
    m_width = width;
    m_height = height;
    m_heightAngle = cameraData.heightAngle;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_pos = cameraData.pos;
    m_look = cameraData.look;
    m_up = cameraData.up;
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(glm::vec3(m_pos),
                       glm::vec3(m_pos + m_look),
                       -glm::vec3(m_up));
}

glm::mat4 Camera::getProjMatrix() {
    float aspectRatio = float(m_width) / m_height;
    float widthAngle = 2 * atan(aspectRatio * tan(m_heightAngle / 2));
    glm::mat4 scale = glm::mat4(1 / (m_farPlane * tan(widthAngle / 2)), 0, 0, 0,
                                0, 1 / (m_farPlane * tan(m_heightAngle / 2)), 0, 0,
                                0, 0, 1 / m_farPlane, 0,
                                0, 0, 0, 1);
    float c = -m_nearPlane / m_farPlane;
    glm::mat4 unhinge = glm::mat4(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1 / (1 + c), -1,
                                  0, 0, -c / (1 + c), 0);
    glm::mat4 remap = glm::mat4(1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, -2, 0,
                                0, 0, -1, 1);
    return remap * unhinge * scale;
}

glm::vec4 Camera::getPos() {
    return m_pos;
}

void Camera::setDimensions(int width, int height) {
    m_width = width;
    m_height = height;
}

void Camera::setPlanes(float nearPlane, float farPlane) {
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void Camera::translate(CameraDirection dir, float deltaTime) {
    if (dir == CameraDirection::FORWARD) {
        m_pos += m_speed * glm::normalize(m_look) * deltaTime;
    } else if (dir == CameraDirection::BACKWARD) {
        m_pos += m_speed * glm::normalize(-m_look) * deltaTime;
    } else if (dir == CameraDirection::LEFT) {
        glm::vec4 direction = glm::vec4(glm::normalize(glm::cross(glm::vec3(m_look), glm::vec3(m_up))), 0.0f);
        m_pos += m_speed * -direction * deltaTime;
    } else if (dir == CameraDirection::RIGHT) {
        glm::vec4 direction = glm::vec4(glm::normalize(glm::cross(glm::vec3(m_look), glm::vec3(m_up))), 0.0f);
        m_pos += m_speed * direction * deltaTime;
    } else if (dir == CameraDirection::UP) {
        glm::vec4 direction = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        m_pos += m_speed * direction * deltaTime;
    } else if (dir == CameraDirection::DOWN) {
        glm::vec4 direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
        m_pos += m_speed * direction * deltaTime;
    }
}

void Camera::rotate(float deltaX, float deltaY) {
    float thetaX = deltaX * M_PI / 1000;
    glm::mat4 rotateX = glm::mat4(cos(thetaX), 0, -sin(thetaX), 0,
                                 0, 1, 0, 0,
                                 sin(thetaX), 0, cos(thetaX), 0,
                                 0, 0, 0, 1);
    float thetaY = deltaY * M_PI / 1000;
    glm::vec3 u = glm::normalize(glm::cross(glm::vec3(m_look), glm::vec3(m_up)));
    glm::mat4 rotateY = glm::mat4(cos(thetaY) + pow(u.x, 2) * (1 - cos(thetaY)),
                                  u.x * u.y * (1 - cos(thetaY)) + u.z * sin(thetaY),
                                  u.x * u.z * (1 - cos(thetaY)) - u.y * sin(thetaY),
                                  0,
                                  u.x * u.y * (1 - cos(thetaY)) - u.z * sin(thetaY),
                                  cos(thetaY) + pow(u.y, 2) * (1 - cos(thetaY)),
                                  u.y * u.z * (1 - cos(thetaY)) + u.x * sin(thetaY),
                                  0,
                                  u.x * u.z * (1 - cos(thetaY)) + u.y * sin(thetaY),
                                  u.y * u.z * (1 - cos(thetaY)) - u.x * sin(thetaY),
                                  cos(thetaY) + pow(u.z, 2) * (1 - cos(thetaY)),
                                  0,
                                  0,
                                  0,
                                  0,
                                  1);
    m_look = rotateY * rotateX * m_look;
}
