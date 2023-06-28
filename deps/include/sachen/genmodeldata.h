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

class Donut{
    public:
    glm::vec3 startpos;
    glm::vec3 fallrichtung;
    int spur;
    int startframe;
    glm::vec3 destpos;
    glm::vec3 currentposition;
    Donut(glm::vec3 startposition, glm::vec3 destpose, glm::vec3 fallrichtunge, int spure, int startframee){ //spur 0 = links
        startpos = startposition;
        currentposition = startposition;
        destpos = destpose;
        fallrichtung = fallrichtunge;
        spur = spure;
        startframe = startframee;
    }
};



#endif
