#ifndef SCENE_H
#define SCENE_H

#include "Ray.h"
#include "ThreeDModel.h"
#include "Triangle.h"
#include "RenderParameters.h"

struct CollisionInfo;

class Scene {
private:
    Material* defaultMaterial;

public:
    std::vector<ThreeDModel>* objects;
    RenderParameters* rp;
    std::vector<Triangle> triangles;

    Scene(std::vector<ThreeDModel>* texobjs, RenderParameters* renderp);

    void updateScene();

    glm::mat4 modelView() const;

    CollisionInfo closestTriangle(const Ray& ray) const;
};

struct CollisionInfo {
    Triangle triangle;
    float t;

    CollisionInfo(const Triangle& triangle, float t);

    bool isHit() const;

    bool isShadowHit() const;
};

#endif // SCENE_H
