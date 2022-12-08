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

    void moveAlongBezierCurve(float distance);

    float calcTFromDistance(float distance);

    void setBezierPoints(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4);

    std::vector<glm::vec3> getNewBezierPoints12();

private:
    int m_width, m_height;
    float m_heightAngle;
    float m_nearPlane, m_farPlane;
    glm::vec4 m_pos, m_look, m_up;
    float m_speed = 5.0f;
    glm::vec3 m_bezierPoint1;
    glm::vec3 m_bezierPoint2;
    glm::vec3 m_bezierPoint3;
    glm::vec3 m_bezierPoint4;
    float m_bezierCurveLength;

    glm::vec3 linearInterpolate(float t, glm::vec3 point1, glm::vec3 point2);

    glm::vec3 calcBezierSegmentPoint(float t);

    glm::vec3 calcBezierSegmentDirection(float t);

    void generateDistanceLUT();

    int m_distanceLUTSize = 20;

    std::vector<float> distanceLUT;
};
