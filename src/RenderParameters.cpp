#include "RenderParameters.h"

RenderParameters::RenderParameters()
    : xTranslate(0.0)
      , yTranslate(0.0)
      , zTranslate(0.0)
      , rotationMatrix(glm::identity<glm::mat4>())
      , interpolationRendering(false)
      , shadowsEnabled(false)
      , fresnelRendering(false)
      , areaLightsEnabled(false)
      , monteCarloEnabled(false)
      , centreObject(false)
      , orthoProjection(false) {
}

void RenderParameters::findLights(const std::vector<ThreeDModel>& objects) {
    for (const ThreeDModel& obj : objects) {
        // find objects that have a "light" material
        if (obj.material->isLight()) {
            // if the object has exactly 2 triangles, its a rectangular area light.
            if (obj.faceVertices.size() == 2) {
                for (unsigned int i = 0; i < 3; i++) {
                    unsigned int vid = obj.faceVertices[0][i];
                    bool found = false;
                    for (unsigned int j = 0; j < 3; j++) {
                        if (vid == obj.faceVertices[1][j]) {
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        unsigned int id1 = obj.faceVertices[0][i];
                        unsigned int id2 = obj.faceVertices[0][(i + 1) % 3];
                        unsigned int id3 = obj.faceVertices[0][(i + 2) % 3];
                        glm::vec3 v1 = obj.vertices[id1];
                        glm::vec3 v2 = obj.vertices[id2];
                        glm::vec3 v3 = obj.vertices[id3];
                        glm::vec3 vecA = v2 - v1;
                        glm::vec3 vecB = v3 - v1;
                        glm::vec4 color = {obj.material->emissive, 1.0f};
                        glm::vec4 pos{v1 + vecA / 2.0f + vecB / 2.0f, 1.0f};
                        glm::vec4 normal{obj.normals[obj.faceNormals[0][0]], 0.0f};
                        Light* l = new Light(Light::Area, color, pos, normal, {vecA, 0.0f}, {vecB, 0.0f});
                        l->enabled = true;
                        lights.push_back(l);
                    }
                }
            } else {
                glm::vec3 center{0.0f};

                for (const auto& vertex : obj.vertices) {
                    center = center + vertex;
                }

                center = center / static_cast<float>(obj.vertices.size());
                Light* l = new Light(
                    Light::Point,
                    {obj.material->emissive, 1.0f},
                    glm::vec4(center, 1.0f),
                    glm::vec4(),
                    glm::vec4(),
                    glm::vec4());
                l->enabled = true;
                lights.push_back(l);
            }
        }
    }

    std::cout << std::endl
            << "Found #" << lights.size() << " lights"
            << std::endl
            << std::endl;
}

RenderParameters::~RenderParameters() {
    for (const auto& light : lights) {
        delete light;
    }
}
