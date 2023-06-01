#ifndef WEGCLASS_H
#define WEGCLASS_H
#include <iostream>
#include <glm/glm.hpp>

class wegdata{
public:
    glm::vec3 rotation;
    glm::vec3 newPos;
    wegdata(glm::vec3 rot, glm::vec3 pos){
        rotation = rot;
        newPos = pos;
    }
};

#endif