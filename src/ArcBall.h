#ifndef ARCBALL_H
#define ARCBALL_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class ArcBall {
private:
    glm::quat baseRotation;
    glm::quat currentRotation;
    glm::quat dragFrom;

public:
    // initializes to a zero rotation
    ArcBall();

    // convert an (x,y) point to a glm::quat
    glm::quat findQuat(float x, float y);

    // start the dragging process with a point in the range [-1..1]^2
    void beginDrag(float x, float y);

    // continue the dragging process with another such point
    void continueDrag(float x, float y);

    // stop dragging
    void endDrag(float x, float y);

    // extract the rotation matrix for rendering purposes
    glm::mat4 rotationMatrix() const;
};

#endif
