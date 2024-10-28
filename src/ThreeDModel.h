#ifndef TEXTURED_OBJECT_H
#define TEXTURED_OBJECT_H

#include <vector>
#include <glm/vec3.hpp>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Material.h"

/*
 * Minimalist (non-optimised) code for reading and
 * rendering an object file
 */
class ThreeDModel {
public:
    std::vector<glm::vec3> vertices;

    std::vector<glm::vec3> normals;

    std::vector<glm::vec3> textureCoords;

    std::vector<std::vector<unsigned int>> faceVertices;

    std::vector<std::vector<unsigned int>> faceNormals;

    std::vector<std::vector<unsigned int>> faceTexCoords;

    Material* material;

    GLuint textureID;

    ThreeDModel();

    static std::vector<ThreeDModel> readObjectStream(std::istream& geometryStream);

    static std::vector<ThreeDModel> readObjectStreamMaterial(std::istream& geometryStream, std::istream& materialStream);

    void writeObjectStream(std::ostream& geometryStream) const;

    void render() const;
};

#endif
