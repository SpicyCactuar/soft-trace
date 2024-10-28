#include "Triangle.h"

#include "Math.h"
#include <glm/ext/quaternion_geometric.hpp>

Triangle::Triangle()
    : vertices({}),
      normals({}),
      colors({}),
      uvs({}),
      sharedMaterial(nullptr) {
}

/**
 * @brief Triangle::intersect indicates the t that satisfies o = s + l*t, if it exists.
 *        The point o is the intersection between the Ray and the Triangle.
 *
 * @param ray the direction used to calculate the intersection
 *
 * @return t > 0.0f if the intersection exists
 */
float Triangle::intersect(const Ray& ray) const {
    glm::vec3 p = perspective(vertices[0]);
    glm::vec3 n = std::get<2>(planarBasis);

    // Get ray properties as {s, l} = {ray.origin, ray.direction}
    glm::vec3 s = ray.origin;
    glm::vec3 l = ray.direction;

    float ln = glm::dot(l, n);

    // If |l.n| < EPS => l.n == 0, therefore the line is parallel to the plane
    // If parallel, either there's no intersection or the whole line intersects
    // There's no "single" point of intersection => indicate there's no intersection
    if (std::abs(ln) < EPS) {
        return NO_INTERSECT;
    }

    // Calculate t such that o = s + t*l, where o intersects with the triangle plane
    // t = (p - s).l / l.n
    float t = glm::dot(p - s, n) / ln;

    // If 0 >= t <=> t <= 0, then the triangle is behind/exactly at the origin of the ray
    // Indicate that there's no intersection
    if (isGreaterEqual(0.0f, t)) {
        return NO_INTERSECT;
    }

    return isInside(s + l * t) ? t : NO_INTERSECT;
}

/**
 * @brief isInside determines whether the target point o is inside the triangle {p, q, r}.
 *        Makes use of u & w, vectors on the plane of the triangle defined by {p, u, w, n}.
 *
 * @param o target point
 *
 * @return true when the point o is inside the triangle defined by {p, q, r},
 *         false otherwise.
 */
bool Triangle::isInside(const glm::vec3& o) const {
    const auto& [u, w, _] = planarBasis;
    const auto& [aPcs, bPcs, cPcs] = pcsVertices;

    glm::vec3 oPcs{glm::dot(o, u), glm::dot(o, w), 0.0f};

    float dot1 = glm::dot(leftOrthogonal(bPcs - aPcs), oPcs - aPcs);
    float dot2 = glm::dot(leftOrthogonal(cPcs - bPcs), oPcs - bPcs);
    float dot3 = glm::dot(leftOrthogonal(aPcs - cPcs), oPcs - cPcs);

    /*
     * Check if the dot products point in the direction of the left orthogonal normals
     * Left-orthogonal normals point inwards
     */
    return isGreaterEqual(dot1, 0.0f) && isGreaterEqual(dot2, 0.0f) && isGreaterEqual(dot3, 0.0f);
}

/**
 * @param o target point
 * @param a point a on the line
 * @param b point b on the line
 *
 * @return the distance from the line (b - a) to the point o
 *         all 3 points are assumed to be on the same plane
 */
float distance(const glm::vec3& o, const glm::vec3& a, const glm::vec3& b) {
    glm::vec3 n = glm::normalize(leftOrthogonal(b - a));

    return glm::dot(n, o) - glm::dot(n, b);
}

/**
 * @param collisionPoint that intersects the triangle
 *
 * @return the (alpha, beta, gamma) barycentric coordinates
 */
glm::vec3 Triangle::barycentricCoordinates(const glm::vec3& collisionPoint) const {
    const auto& [u, w, _] = planarBasis;
    const auto& [aPcs, bPcs, cPcs] = pcsVertices;
    glm::vec3 oPcs{glm::dot(collisionPoint, u), glm::dot(collisionPoint, w), 0.0f};

    float alpha = distance(oPcs, bPcs, cPcs) / distance(aPcs, bPcs, cPcs);
    float beta = distance(oPcs, aPcs, cPcs) / distance(bPcs, aPcs, cPcs);
    float gamma = distance(oPcs, bPcs, aPcs) / distance(cPcs, bPcs, aPcs);

    return {alpha, beta, gamma};
}

void Triangle::computePlanarValues() {
    // Get vertices as Cartesian3 points
    glm::vec3 a = vertices[0];
    glm::vec3 b = vertices[1];
    glm::vec3 c = vertices[2];

    // Compute vectors u & v
    // We only need to normalise one of them to compute the orthonormal basis
    // Choose u
    glm::vec3 u = glm::normalize(b - a);
    glm::vec3 v = c - a;

    // Since u and v are parallel to the plane, u x v is normal
    glm::vec3 n = glm::normalize(glm::cross(u, v));
    // Now that we have 2 orthonormal vectors, we can compute the third
    glm::vec3 w = glm::normalize(glm::cross(n, u));

    planarBasis = std::make_tuple(u, w, n);

    // Project all the points onto the triangle's PCS
    // z = 0.0f for all points given they will be projected onto the plane they are on
    glm::vec3 aPcs{glm::dot(a, u), glm::dot(a, w), 0.0f};
    glm::vec3 bPcs{glm::dot(b, u), glm::dot(b, w), 0.0f};
    glm::vec3 cPcs{glm::dot(c, u), glm::dot(c, w), 0.0f};

    pcsVertices = std::make_tuple(aPcs, bPcs, cPcs);
}

glm::vec3 Triangle::weightedNormal(const glm::vec3& weights) const {
    return weights[0] * normals[0] + weights[1] * normals[1] + weights[2] * normals[2];
}
