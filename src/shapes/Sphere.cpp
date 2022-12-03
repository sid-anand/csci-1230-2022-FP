#include "Sphere.h"

void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    glm::vec3 topLeftNorm = glm::normalize(topLeft);
    glm::vec3 bottomLeftNorm = glm::normalize(bottomLeft);
    glm::vec3 bottomRightNorm = glm::normalize(bottomRight);
    glm::vec3 topRightNorm = glm::normalize(topRight);

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topLeftNorm);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, bottomLeftNorm);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, bottomRightNorm);

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topLeftNorm);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, bottomRightNorm);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, topRightNorm);
}

void Sphere::setVertexData() {
    auto spherical = [](float theta, float phi) {
        return glm::vec3{
            0.5f * sin(phi) * sin(theta),
            0.5f * cos(phi),
            0.5f * sin(phi) * cos(theta)
        };
    };

    int param1 = std::max(m_param1, 2);
    int param2 = std::max(m_param2, 3);

    float thetaStep = glm::radians(360.f / param2);
    float phiStep = glm::radians(180.f / param1);

    for (int i = 0; i < param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = currentTheta + thetaStep;
        for (int i = 0; i < param1; i++) {
            float currentPhi = i * phiStep;
            float nextPhi = currentPhi + phiStep;
            makeTile(spherical(currentTheta, currentPhi),
                     spherical(nextTheta, currentPhi),
                     spherical(currentTheta, nextPhi),
                     spherical(nextTheta, nextPhi));
        }
    }
}

void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
