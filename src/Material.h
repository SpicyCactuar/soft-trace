#ifndef MATERIAL_H
#define MATERIAL_H

#include <fstream>
#include <string>
#include <glm/vec3.hpp>
#include <vector>

#include "RGBAImage.h"

// Material based on .mtl + custom properties (N_ior, N_mirr, N_transp)
class Material {
public:
    bool setFromFile;
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emissive;
    float shininess;
    // stuff for reflection and refraction
    float reflectivity;
    float indexOfRefraction;
    float transparency;
    RGBAImage* texture;

    bool isLight() const;

    Material();

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emissive, float shininess,
             std::istream& textureStream);

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emissive, float shininess);

    // no texture in constructor;
    ~Material();

    static std::vector<Material*> readMaterials(std::istream& materialStream);
};

#endif // MATERIAL_H
