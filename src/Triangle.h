#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <array>
#include <tuple>
#include <glm/vec4.hpp>

#include "Material.h"
#include "Ray.h"

class Triangle {
public:
    std::array<glm::vec4, 3> vertices;
    std::array<glm::vec4, 3> normals;
    std::array<glm::vec4, 3> colors;

    std::array<glm::vec3, 3> uvs;

    Material* sharedMaterial;

    Triangle();

    void computePlanarValues();

    float intersect(const Ray& ray) const;

    glm::vec3 barycentricCoordinates(const glm::vec3& collisionPoint) const;

    glm::vec3 weightedNormal(const glm::vec3& weights) const;

private:
    // Orthonormal basis of the plane of the triangle [u w n]
    // u & w are pallalel to the plane - n is normal to the plane
    std::tuple<glm::vec3, glm::vec3, glm::vec3> planarBasis;

    // vertices as PCS coordinates with respect to the triangle's plane
    std::tuple<glm::vec3, glm::vec3, glm::vec3> pcsVertices;

    bool isInside(const glm::vec3& o) const;
};

#endif // TRIANGLE_H
