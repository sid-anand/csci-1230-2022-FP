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

glm::vec3 Camera::linearInterpolate(float t, glm::vec3 point1, glm::vec3 point2) {
    return point1 * (1 - t) + point2 * t;
}

glm::vec3 Camera::calcBezierSegmentPoint(float t) {
    glm::vec3 interpolated12 = linearInterpolate(t, m_bezierPoint1, m_bezierPoint2);
    glm::vec3 interpolated23 = linearInterpolate(t, m_bezierPoint2, m_bezierPoint3);
    glm::vec3 interpolated34 = linearInterpolate(t, m_bezierPoint3, m_bezierPoint4);
    glm::vec3 interpolated123 = linearInterpolate(t, interpolated12, interpolated23);
    glm::vec3 interpolated234 = linearInterpolate(t, interpolated23, interpolated34);
    glm::vec3 final = linearInterpolate(t, interpolated123, interpolated234);
    return final;
}

glm::vec3 Camera::calcBezierSegmentDirection(float t) {
    float tSquared = float(pow(t, 2));
    glm::vec3 p1Vals = m_bezierPoint1 * (-3.f * tSquared + 6.f * t - 3.f);
    glm::vec3 p2Vals = m_bezierPoint2 * ( 9.f * tSquared - 12.f * t + 3);
    glm::vec3 p3Vals = m_bezierPoint3 * (-9.f * tSquared + 6.f * t);
    glm::vec3 p4Vals = m_bezierPoint4 * ( 3.f * tSquared);
    glm::vec3 derivative = p1Vals + p2Vals + p3Vals + p4Vals;
    glm::vec3 direction = glm::normalize(derivative);
    return direction;
}

void Camera::moveAlongBezierCurve(float distance) {
    float t = calcTFromDistance(distance);
    m_pos = glm::vec4(calcBezierSegmentPoint(t), 1);
    m_look = glm::vec4(calcBezierSegmentDirection(t), 1);
}

float Camera::calcTFromDistance(float distance) {
    float t1 = std::floor(distance * m_distanceLUTSize) / float(m_distanceLUTSize);
    float t2 = std::ceil(distance * m_distanceLUTSize) / float(m_distanceLUTSize);
    if (t2 == 0) {
        return 0;
    } else if (t1 == 1) {
        return 1;
    } else {
        float difference = (distance - t1) * m_distanceLUTSize;
        int index = t1 * m_distanceLUTSize;
        float priorDistance = distanceLUT[index];
        float nextDistance = distanceLUT[index + 1];
        float added = (nextDistance - priorDistance) * difference;
        return (priorDistance + added) / m_bezierCurveLength;
    }
}

void Camera::generateDistanceLUT() {
    distanceLUT.clear();
    distanceLUT.push_back(0);
    glm::vec3 oldPoint = m_bezierPoint1;
    glm::vec3 newPoint;
    float curDistance = 0;
    for (int i = 1; i <= m_distanceLUTSize; i++) {
        float t = float(i) / float(m_distanceLUTSize);
        newPoint = calcBezierSegmentPoint(t);
        curDistance += glm::distance(newPoint, oldPoint);
        distanceLUT.push_back(curDistance);
        oldPoint = newPoint;
    }
    m_bezierCurveLength = curDistance;
}

void Camera::setBezierPoints(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4) {
    m_bezierPoint1 = point1;
    m_bezierPoint2 = point2;
    m_bezierPoint3 = point3;
    m_bezierPoint4 = point4;
    generateDistanceLUT();
}

std::vector<glm::vec3> Camera::getNewBezierPoints12() {
    glm::vec3 newp1 = m_bezierPoint4;
    glm::vec3 newp2 = (2.f * m_bezierPoint4 - m_bezierPoint3);
    return std::vector<glm::vec3> {newp1, newp2};
}
