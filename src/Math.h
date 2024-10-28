#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <limits>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

constexpr float EPS = std::numeric_limits<float>::epsilon();
constexpr float NO_INTERSECT = -1.0f;

bool isGreaterEqual(float a, float b);

glm::vec3 leftOrthogonal(const glm::vec3&);

glm::vec3 perspective(const glm::vec4& homogeneous);

glm::vec4 modulate(const glm::vec4& value, const glm::vec4& factor);

std::istream& operator>>(std::istream& is, glm::vec3& vec);

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);

#endif // MATH_H
