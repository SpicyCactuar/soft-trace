#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec4.hpp>

class Light {
public:
    glm::vec4 lightPosition;
    glm::vec4 lightDirection;
    glm::vec4 lightColor;

    enum LightType {
        Point,
        Area,
        Directional
    };

private:
    glm::vec4 tangent1;
    glm::vec4 tangent2;
    LightType type;

public:
    Light(
        LightType type,
        glm::vec4 color,
        glm::vec4 pos,
        glm::vec4 dir,
        glm::vec4 tan1,
        glm::vec4 tan2);

    bool enabled;

    glm::vec4 sampledPosition() const;
};

#endif // LIGHT_H
