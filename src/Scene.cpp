#include "Scene.h"

#include "Math.h"
#include <limits>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Scene::Scene(std::vector<ThreeDModel>* texobjs, RenderParameters* renderp) {
    objects = texobjs;
    rp = renderp;

    glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 emissive = glm::vec3(0, 0, 0);
    float shininess = 1.0f;

    defaultMaterial = new Material(ambient, diffuse, specular, emissive, shininess);
}

void printModelView(const glm::mat4& modelView) {
    std::cout << std::endl
            << "ModelView Matrix:" << std::endl;

    std::cout << std::endl
            << glm::to_string(modelView)
            << std::endl;
}

void Scene::updateScene() {
    triangles.clear(); // Clear the list so it can be populated again

    typedef unsigned int uint;

    glm::mat4 modelView = this->modelView();

    printModelView(modelView);

    for (const auto& object : *objects) {
        for (uint face = 0; face < object.faceVertices.size(); face++) {
            for (uint triangle = 0; triangle < object.faceVertices[face].size() - 2; triangle++) {
                Triangle t;

                for (uint vertex = 0; vertex < 3; vertex++) {
                    uint faceVertex = 0;
                    if (vertex != 0) {
                        faceVertex = triangle + vertex;
                    }

                    auto v = glm::vec4(
                        object.vertices[object.faceVertices[face][faceVertex]].x,
                        object.vertices[object.faceVertices[face][faceVertex]].y,
                        object.vertices[object.faceVertices[face][faceVertex]].z,
                        1.0f);
                    t.vertices[vertex] = modelView * v;

                    auto n = glm::vec4(
                        object.normals[object.faceNormals[face][faceVertex]].x,
                        object.normals[object.faceNormals[face][faceVertex]].y,
                        object.normals[object.faceNormals[face][faceVertex]].z,
                        0.0f);
                    t.normals[vertex] = modelView * n;

                    auto tex = glm::vec3(
                        object.textureCoords[object.faceTexCoords[face][faceVertex]].x,
                        object.textureCoords[object.faceTexCoords[face][faceVertex]].y,
                        0.0f);
                    t.uvs[vertex] = tex;

                    t.colors[vertex] = {0.7f, 0.7f, 0.7f, 1.0f};
                }

                if (object.material == nullptr) {
                    t.sharedMaterial = defaultMaterial;
                } else {
                    t.sharedMaterial = object.material;
                }

                t.computePlanarValues();
                triangles.push_back(t);
            }
        }
    }
}

glm::mat4 Scene::modelView() const {
    glm::mat4 rotation = rp->rotationMatrix;
    // Offset scene z by (-1) to mimic RenderWidget
    auto translation = glm::identity<glm::mat4>();
    translation = glm::translate(translation, {rp->xTranslate, rp->yTranslate, rp->zTranslate - 1.0f});
    // We are using this matrix for pre-multiplication
    // M*v = T*R*v
    return translation * rotation;
}

CollisionInfo Scene::closestTriangle(const Ray& ray) const {
    const Triangle* closestTriangle = nullptr;
    float minT = std::numeric_limits<float>::infinity();

    for (auto& triangle : triangles) {
        float t = triangle.intersect(ray);

        if (0.0f < t && t < minT) {
            closestTriangle = &triangle;
            minT = t;
        }
    }

    return closestTriangle != nullptr
               ? CollisionInfo(*closestTriangle, minT)
               : CollisionInfo(Triangle(), NO_INTERSECT);
}

CollisionInfo::CollisionInfo(const Triangle& triangle, float t)
    : triangle(triangle)
      , t(t) {
}

bool CollisionInfo::isHit() const {
    return t > 0.0f;
}

bool CollisionInfo::isShadowHit() const {
    return isHit() && !triangle.sharedMaterial->isLight();
}
