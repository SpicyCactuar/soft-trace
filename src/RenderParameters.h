#ifndef RENDER_PARAMETERS_H
#define RENDER_PARAMETERS_H

#include <vector>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Light.h"
#include "ThreeDModel.h"

class RenderParameters {
public:
    float xTranslate, yTranslate, zTranslate;

    glm::mat4 rotationMatrix;

    bool interpolationRendering;
    bool shadowsEnabled;
    bool fresnelRendering;
    bool areaLightsEnabled;
    bool monteCarloEnabled;

    bool centreObject;

    bool orthoProjection;

    std::vector<Light*> lights;

    RenderParameters();

    void findLights(const std::vector<ThreeDModel>& objects);

    ~RenderParameters();
};

// now define some macros for bounds on parameters
#define TRANSLATE_MIN (-1.0f)
#define TRANSLATE_MAX 1.0f

#define ZOOM_SCALE_LOG_MIN (-2.0f)
#define ZOOM_SCALE_LOG_MAX 2.0f
#define ZOOM_SCALE_MIN 0.01f
#define ZOOM_SCALE_MAX 100.0f

#define LIGHTING_MIN 0.0f
#define LIGHTING_MAX 1.0f

#define SPECULAR_EXPONENT_LOG_MIN (-2.0f)
#define SPECULAR_EXPONENT_LOG_MAX 2.0f
#define SPECULAR_EXPONENT_MIN 0.01f
#define SPECULAR_EXPONENT_MAX 100.0f

// this is to scale to/from integer values
#define PARAMETER_SCALING 100

#endif
