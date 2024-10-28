#ifndef SURFACEELEMENT_H
#define SURFACEELEMENT_H

#include "Triangle.h"

class SurfaceElement {
public:
    SurfaceElement(
        const Triangle& triangle,
        const glm::vec3& point,
        const glm::vec3& normal);

    const Triangle triangle;
    const glm::vec3 point;
    const glm::vec3 normal;

    glm::vec4 directLighting(
        const glm::vec4& lightPosition,
        const glm::vec4& lightColour,
        const glm::vec4& eye) const;

    glm::vec4 indirectLighting() const;

    glm::vec4 emissive() const;

    bool isPhong() const;

    float indexOfRefraction() const;

    float schlick(const Ray& ray, float indexOfRefraction) const;
};

#endif // SURFACEELEMENT_H
