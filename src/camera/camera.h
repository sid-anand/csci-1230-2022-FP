#pragma once

#include "utils/scenedata.h"
#include <glm/glm.hpp>

enum class CameraDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera();

    Camera(int width, int height, const SceneCameraData &cameraData, float nearPlane, float farPlane);

    glm::mat4 getViewMatrix();

    glm::mat4 getProjMatrix();

    glm::vec4 getPos();

    void setDimensions(int width, int height);

    void setPlanes(float nearPlane, float farPlane);

    void translate(CameraDirection dir, float deltaTime);

    void rotate(float deltaX, float deltaY);

private:
    int m_width, m_height;
    float m_heightAngle;
    float m_nearPlane, m_farPlane;
    glm::vec4 m_pos, m_look, m_up;
    float m_speed = 5.0f;
};
