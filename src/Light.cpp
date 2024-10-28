#include "Light.h"

#include "Math.h"

Light::Light(LightType type, glm::vec4 color, glm::vec4 pos, glm::vec4 dir, glm::vec4 tan1, glm::vec4 tan2) {
    this->type = type;
    this->lightPosition = pos;
    this->lightDirection = dir;
    this->tangent1 = tan1;
    this->tangent2 = tan2;
    this->lightColor = color;
    enabled = false;
}

glm::vec4 Light::sampledPosition() const {
    const float u = (static_cast<float>(random()) / static_cast<float>(RAND_MAX) - 0.5f) * 0.5f;
    const float v = (static_cast<float>(random()) / static_cast<float>(RAND_MAX) - 0.5f) * 0.5f;

    return lightPosition + (u * tangent1 + v * tangent2);
}
