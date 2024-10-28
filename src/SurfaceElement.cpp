#include "SurfaceElement.h"

#include "Math.h"
#include <limits>
#include <glm/ext/quaternion_geometric.hpp>

SurfaceElement::SurfaceElement(
    const Triangle& triangle,
    const glm::vec3& point,
    const glm::vec3& normal)
    : triangle(triangle)
      , point(point)
      , normal(normal) {
}

/**
 *
 * @param lightPosition homogeneous position of the light
 * @param lightColour colour of the light
 * @param eye the homogeneous position of the eye
 *
 * @return the blinn-phong colour resulting from a direct lighting contribution
 */
glm::vec4 SurfaceElement::directLighting(const glm::vec4& lightPosition,
                                         const glm::vec4& lightColour,
                                         const glm::vec4& eye) const {
    // Light direction
    glm::vec3 vl = static_cast<glm::vec3>(lightPosition) - this->point;

    // If the triangle is not facing the light, do not apply specular and diffuse
    bool isPointFacingLight = isGreaterEqual(glm::dot(normal, vl), 0.0f);
    if (!isPointFacingLight) {
        return glm::vec4(0.0f);
    }

    // Attenuation
    float distance = glm::length(vl);
    float attenuationFactor = 1.0f / (1.0f + 1.0f * distance * distance);
    glm::vec4 attenuation{attenuationFactor, attenuationFactor, attenuationFactor, 1.0f};

    // Specular
    glm::vec3 ve = static_cast<glm::vec3>(eye) - this->point;
    // No need to divide by 2 since it is normalised later
    glm::vec3 vb = vl + ve;
    float cosThetaSpecular = std::pow(std::max(glm::dot(normal, glm::normalize(vb)), 0.0f),
                                      triangle.sharedMaterial->shininess);
    glm::vec4 specularFactor{cosThetaSpecular * triangle.sharedMaterial->specular, 1.0f};
    glm::vec4 specular = lightColour * specularFactor;

    // Diffuse
    float cosThetaDiffuse = std::max(glm::dot(normal, glm::normalize(vl)), 0.0f);
    glm::vec4 diffuseFactor{cosThetaDiffuse * triangle.sharedMaterial->diffuse, 1.0f};
    glm::vec4 diffuse = lightColour * diffuseFactor;

    return (diffuse + specular) * attenuation;
}

/**
 * @return the colour from the surface light emission
 */
glm::vec4 SurfaceElement::emissive() const {
    // Emissive
    return {triangle.sharedMaterial->emissive, 1.0f};
}

/**
 * @return the colour from indirect lighting contributions
 */
glm::vec4 SurfaceElement::indirectLighting() const {
    // Ambient
    return {triangle.sharedMaterial->ambient, 1.0f};
}

/**
 * @return whether rays should either refract and/or reflect on the surface
 */
bool SurfaceElement::isPhong() const {
    // clang-format off
    return triangle.sharedMaterial->reflectivity == 0.0f &&
           triangle.sharedMaterial->transparency == 0.0f;
    // clang-format on
}

float SurfaceElement::indexOfRefraction() const {
    return triangle.sharedMaterial->indexOfRefraction;
}

float SurfaceElement::schlick(const Ray& ray, float mediumRefractiveIndex) const {
    float cosTheta1 = -glm::dot(ray.direction, normal);

    float n1 = mediumRefractiveIndex;
    float n2 = indexOfRefraction();

    // Not the code path that we deserve, but the one we need
    // If same refractive index, all energy is refracted
    // Avoids issues with division by 0 in the ft calculation
    if (std::abs(n1 - n2) < std::numeric_limits<float>::epsilon()) {
        return 0.0f;
    }

    float n = n1 / n2;
    float sinThetaSquared = n * n * (1.0f - cosTheta1 * cosTheta1);
    // Total internal reflection, cannot refract
    if (sinThetaSquared > 1.0f) {
        return 1.0f;
    }

    float ft = (n1 - n2) / (n1 + n2);
    ft *= ft;

    // Schlick's Approximation
    return ft + (1.0f - ft) * std::pow(1.0f - cosTheta1, 5.0f);
}
