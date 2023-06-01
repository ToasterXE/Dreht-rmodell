#ifndef GENMODELDATA_H
#define GENMODELDATA_H

#include <glm/glm.hpp>
#include <iostream>
#include <time.h>

class modelData{
    public:
    float rotSpeed;
    glm::vec3 pos;
    float scale;
    glm::vec3 color;
    modelData(float rotationSpeed, glm::vec3 position, float scalexyz, glm::vec3 colorval){
        rotSpeed = rotationSpeed;
        pos = position;
        scale = scalexyz;
        color = colorval;
    }
};

#endif
