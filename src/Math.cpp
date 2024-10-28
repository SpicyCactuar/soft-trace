#include "Math.h"

bool isGreaterEqual(float a, float b) {
    return std::abs(a - b) < EPS || a > b;
}

glm::vec3 leftOrthogonal(const glm::vec3& vector) {
    return {-vector.y, vector.x, vector.z};
}

glm::vec3 perspective(const glm::vec4& homogeneous) {
    return {
        homogeneous.x / homogeneous.w,
        homogeneous.y / homogeneous.w,
        homogeneous.z / homogeneous.w
    };
}

std::istream& operator>>(std::istream& is, glm::vec3& vec) {
    return is >> vec.x >> vec.y >> vec.z;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    return os << vec.x << " " << vec.y << " " << vec.z;
}
