#pragma once

#include <vector>
#include <glm/glm.hpp>

class Ground
{
public:
    void updateParams(int param1, int size, int depth);
    std::vector<float> getMesh() { return m_vertexData; }

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void makeGround();

    std::vector<float> m_vertexData;
    int m_param1;
    int m_size;
    int m_depth;
};
