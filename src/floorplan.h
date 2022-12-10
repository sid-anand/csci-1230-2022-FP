#include "shapes/Building.h"
#include <iostream>
#include <ostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class FloorPlan {
    public:
        std::vector<Building> buildings;
        const float streetSize = 1.0f;
        const int maxHeight = 10;
        const int maxBlockSize = 4;
        std::vector<int> blockSizesX;
        std::vector<int> blockSizesZ;
        int blockSize = 4;

        FloorPlan(float gridSize, int numTextures) {
            blockSizesX.clear();
            blockSizesZ.clear();
            // randomly generate x dimension from 2-5
            // randomly generate z dimension from 2-5
            // if there's enough space in that dimension, use it
//            float currentSizeX = 0;
//            while (currentSizeX < gridSize) {
//                int blockSize = (arc4random() % maxBlockSize) + 2;
//                while (currentSizeX + blockSize > gridSize) {
//                    blockSize -= 1;
//                }
//                if (blockSize < 2) {
//                    break;
//                }
//                blockSizesX.push_back(blockSize);
//                currentSizeX += (blockSize + streetSize);
//            }
//            float currentSizeZ = 0;
//            while (currentSizeZ < gridSize) {
//                int blockSize = (arc4random() % maxBlockSize) + 2;
//                while (currentSizeZ + blockSize > gridSize) {
//                    blockSize -= 1;
//                }
//                if (blockSize < 2) {
//                    break;
//                }
//                blockSizesZ.push_back(blockSize);
//                currentSizeZ += (blockSize + streetSize);

//            }
            blockSizesX.push_back(4);
            blockSizesX.push_back(4);
            blockSizesX.push_back(4);
            blockSizesX.push_back(4);

            blockSizesZ.push_back(2);
            blockSizesZ.push_back(2);
            blockSizesZ.push_back(2);
            blockSizesZ.push_back(2);
            // vector 1 is x direction, its like [1, 4, 3, 1, 1]
            // vector 2 is z direction, like     [2, 2, 4, 2]
            // xAccumulator and zAccumulator are basically the coordinates of where the block is
            // note it starts at -streetSize rather than 0 because we add streetSize + the buildingSize in the for loop,
            // and we don't have a street for the first block in either dimension
            float xAccumulator = 0;
            int counter = 0;
            int texturenum = 0;
            for (int i = 0; i < blockSizesX.size(); i++) {
                float buildingDimensionX = blockSizesX[i];
                float zAccumulator = 0;
                for (int j = 0; j < blockSizesZ.size(); j++) {
                    float buildingHeight = arc4random() % maxHeight + 1;
                    Building building = Building();
                    float buildingDimensionZ = blockSizesZ[j];
                    counter++;
                    zAccumulator -= (buildingDimensionZ + streetSize);
                    if (counter % 2) {
                        continue;
                    }
                    texturenum += 1;
                    if (texturenum >= 8) {
                        texturenum = 0;
                    }
                    building.updateParams(1, buildingDimensionX, buildingHeight, buildingDimensionZ, xAccumulator, zAccumulator, texturenum);
                    buildings.push_back(building);
                }
                xAccumulator += (buildingDimensionX + streetSize);
            }


//            for (float bi = 0.f; bi <= gridSize; bi += (blockSize + streetSize)) {
//                for (float bk = 0.f; bk >= -gridSize; bk -= (blockSize + streetSize)) {
//                    float buildingHeight = arc4random() % maxHeight + 1;
//                    Building building = Building();
//                    building.updateParams(1, 5, buildingHeight, 5, bi, bk, arc4random() % numTextures);
//                    buildings.push_back(building);
//                }
//            }
        }





};
