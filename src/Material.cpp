#include "Material.h"

#include <string>
#include "Math.h"

Material::Material(glm::vec3 ambient,
                   glm::vec3 diffuse,
                   glm::vec3 specular,
                   glm::vec3 emissive,
                   float shininess,
                   std::istream& textureStream) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->emissive = emissive;
    this->shininess = shininess;
    this->reflectivity = 0;
    this->indexOfRefraction = 1;
    this->transparency = 0;
    texture = new RGBAImage();
    texture->readPPM(textureStream);
    name = "default";
    setFromFile = false;
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emissive, float shininess) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->emissive = emissive;
    this->shininess = shininess;
    this->reflectivity = 0;
    this->indexOfRefraction = 1;
    this->transparency = 0;
    texture = nullptr;
    name = "default";
    setFromFile = false;
}

Material::Material() {
    this->ambient = glm::vec3();
    this->diffuse = glm::vec3();
    this->specular = glm::vec3();
    this->emissive = glm::vec3();
    this->shininess = 0.0f;
    this->reflectivity = 0;
    this->indexOfRefraction = 1;
    this->transparency = 0;
    texture = nullptr;
    name = "default";
    setFromFile = false;
}

Material::~Material() {
    delete texture;
}

std::vector<Material*> Material::readMaterials(std::istream& materialStream) {
    std::vector<Material*> r;
    Material* m = new Material();

    std::string name = "";
    // the rest of this is a loop reading lines & adding them in appropriate places
    while (true) {
        // check for eof() in case we've run out
        if (materialStream.eof())
            break;

        std::string token;
        materialStream >> token;
        if (token == "newmtl") {
            if (!name.empty()) {
                m->setFromFile = true;
                r.push_back(m);
                m = new Material();
            }
            materialStream >> name;
            m->name = name;
        } else if (token == "Ka") {
            glm::vec3 ambient;
            materialStream >> ambient;
            m->ambient = ambient;
        } else if (token == "Kd") {
            glm::vec3 diffuse;
            materialStream >> diffuse;
            m->diffuse = diffuse;
        } else if (token == "Ks") {
            glm::vec3 specular;
            materialStream >> specular;
            m->specular = specular;
        } else if (token == "Ke") {
            glm::vec3 emissive;
            materialStream >> emissive;
            m->emissive = emissive;
        } else if (token == "Ns") {
            float shininess;
            materialStream >> shininess;
            m->shininess = shininess;
        } else if (token == "N_ior") {
            float ior;
            materialStream >> ior;
            m->indexOfRefraction = ior;
        } else if (token == "N_mirr") {
            float reflect;
            materialStream >> reflect;
            m->reflectivity = reflect;
        } else if (token == "N_transp") {
            float transp;
            materialStream >> transp;
            m->transparency = transp;
        } else if (token == "map_Ka") {
            std::string filename;
            materialStream >> filename;
            std::ifstream textureFile(filename.c_str());
            if (!textureFile.good()) {
                std::cout << "Problem reading texture " << filename << " for the material " << m->name << std::endl;
            } else {
                m->texture = new RGBAImage();
                m->texture->readPPM(textureFile);
            }
        }
    }
    m->setFromFile = true;
    r.push_back(m);
    return r;
}

bool Material::isLight() const {
    return name.find("light") != std::string::npos;
}
