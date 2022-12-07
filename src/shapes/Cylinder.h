#pragma once

#include <vector>
#include <glm/glm.hpp>

class Cylinder
{
public:
    void updateParams(int param1, int param2, float x, float y, float z, float radius);
    std::vector<float> getMesh() { return m_vertexData; }

private:
    void insertVec2(std::vector<float> &data, glm::vec2 v);
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight,
                  int cap);

    std::vector<float> m_vertexData;
    int m_param1;
    int m_param2;
    float m_x;
    float m_y;
    float m_z;
    float m_radius;
};
