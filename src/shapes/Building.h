#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include <glm/glm.hpp>


class Building
{
public:
    void updateParams(int param1, int size, int height, float x, float z, int texture);
    std::vector<float> getMesh() { return m_vertexData; }

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void makeBuilding();

    std::vector<float> m_vertexData;
    int m_param1;
    int m_size;
    int m_height;
    float m_x;
    float m_z;
    int m_texture;
};


#endif // BUILDING_H
