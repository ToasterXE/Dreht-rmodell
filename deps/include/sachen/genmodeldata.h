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
    glm::vec3 e;
    modelData(glm::vec3 originalposition, float rotationSpeed, glm::vec3 position, float scalexyz, glm::vec3 colorval){
        rotSpeed = rotationSpeed;
        e = originalposition;
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
    glm::vec3 rotation;
    Donut(glm::vec3 startposition, glm::vec3 destpose, glm::vec3 fallrichtunge, int spure, int startframee){ //spur 0 = links
        startpos = startposition;
        currentposition = startposition;
        destpos = destpose;
        fallrichtung = fallrichtunge;
        spur = spure;
        startframe = startframee;
        rotation=glm::vec3(float(rand()%180),float(rand()%180),float(rand()%180));
    }
};



#endif
