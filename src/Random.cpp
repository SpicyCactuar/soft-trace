#include "Random.h"

#include <random>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>

glm::vec3 randomMonteCarloDirection(const glm::vec3& normal) {
    float randomCos = static_cast<float>(random()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
    float randomPhi = static_cast<float>(random()) / static_cast<float>(RAND_MAX);

    // Generate a random direction in the hemisphere
    float theta = std::acos(randomCos);
    float phi = 2.0 * M_PI * randomPhi;

    // Convert spherical coordinates to Cartesian coordinates
    float x = std::sin(theta) * std::cos(phi);
    float y = std::sin(theta) * std::sin(phi);
    float z = std::cos(theta);

    // Compute axes of rotation matrix
    glm::vec3 n = glm::normalize(normal);
    glm::vec3 nt = glm::vec3(n.z, 0, -n.x);
    // Case in which n = (0, 1, 0)
    if (std::abs(glm::length(nt)) < std::numeric_limits<float>::epsilon()) {
        nt = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    glm::vec3 ns = glm::cross(nt, n);

    // Create rotation matrix
    auto r = glm::identity<glm::mat3>();

    // Set nt as first column
    r[0][0] = nt[0];
    r[1][0] = nt[1];
    r[2][0] = nt[2];

    // Set n as second column
    r[0][1] = n[0];
    r[1][1] = n[1];
    r[2][1] = n[2];

    // Set ns as third column
    r[0][2] = ns[0];
    r[1][2] = ns[1];
    r[2][2] = ns[2];

    // Transform the direction to the local coordinate system
    return r * glm::vec3(x, y, z);
}
