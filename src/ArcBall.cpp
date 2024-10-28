#include "ArcBall.h"

#include <cmath>

ArcBall::ArcBall() {
    // set to (0, 0, 0, 1) - no rotation
    baseRotation = glm::quat(1, 0, 0, 0);
    currentRotation = baseRotation;
    dragFrom = glm::quat(0, 0, 1, 0);
}

// convert an (x,y) point to a quaternion
glm::quat ArcBall::findQuat(const float x, const float y) {
    glm::vec3 result;
    float dSquared = x * x + y * y;
    if (dSquared > 1.0f) {
        // outside the circle, so
        // constrain to the circle with scaling
        result = glm::vec3(x / std::sqrt(dSquared), y / std::sqrt(dSquared), 0.0f);
    } else {
        // inside the circle, so 1-dSquared = z squared
        result = glm::vec3(x, y, std::sqrt(1.0f - dSquared));
    }
    return {0.0f, -result.x, -result.y, result.z};
}

// start the dragging process with a point in the range [-1..1]^2
void ArcBall::beginDrag(float x, float y) {
    // convert the initial point to a quaternion
    dragFrom = findQuat(x, y);
}

// continue the dragging process with another such point
void ArcBall::continueDrag(float x, float y) {
    // find the quaternion representing the point
    glm::quat nextQuat = findQuat(x, y);
    // invert the previous quaternion
    glm::quat fromInverse = glm::inverse(dragFrom);
    // use it to compute the current rotation
    currentRotation = nextQuat * fromInverse;
}

// stop dragging
void ArcBall::endDrag(float x, float y) {
    // find the quaternion representing the point
    glm::quat nextQuat = findQuat(x, y);
    // invert the previous quaternion
    glm::quat fromInverse = glm::inverse(dragFrom);
    // use it to compute the current rotation
    currentRotation = nextQuat * fromInverse;

    // now reset the quaternions ready for next time
    dragFrom = glm::quat(0, 0, 1, 0);
    // and reset current and base
    baseRotation = currentRotation * baseRotation;
    currentRotation = glm::quat(1, 0, 0, 0);
}

glm::mat4 ArcBall::rotationMatrix() const {
    glm::mat4 currentMatrix = glm::mat4_cast(currentRotation);
    glm::mat4 baseMatrix = glm::mat4_cast(baseRotation);
    return currentMatrix * baseMatrix;
}
