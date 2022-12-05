//#pragma once

//#include <vector>
//#include <glm/glm.hpp>

//class Building
//{
//public:
//    void updateParams(int param1, int size, int height);
//    std::vector<float> generate() { return m_vertexData; }

//private:
//    void insertVec3(std::vector<float> &data, glm::vec3 v);
//    void setVertexData();

//    std::vector<float> m_vertexData;
//    int m_param1;
//    int m_size;
//    int m_height;
//};

#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include <glm/glm.hpp>


class Building
{
public:
    void updateParams(int param1, int size, int height);
    std::vector<float> getMesh() { return m_vertexData; }

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void makeBuilding();

    std::vector<float> m_vertexData;
    int m_param1;
    int m_size;
    int m_height;
};


#endif // BUILDING_H
