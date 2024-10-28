#ifndef RAY_H
#define RAY_H

#include <glm/vec3.hpp>

class Ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(glm::vec3 origin, glm::vec3 direction);
};

#endif // RAY_H
