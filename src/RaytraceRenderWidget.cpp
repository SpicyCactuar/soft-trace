#include "RaytraceRenderWidget.h"

#include <QTimer>
#include <cmath>
#include <random>
#include <ext/matrix_transform.hpp>
#include <gtx/string_cast.hpp>

#include "SurfaceElement.h"
#include "Random.h"

#define N_THREADS 16
#define N_LOOPS 100
#define N_BOUNCES 5
#define N_MC_SAMPLES 4
#define N_AA_SAMPLES 10
#define N_SS_SAMPLES 20
#define TERMINATION_FACTOR 0.35f

constexpr glm::vec3 camera{0.0f};
constexpr float collisionBias = 0.001f;
constexpr float airRefractiveIndex = 1.0003f;

constexpr glm::vec4 NoColour{0.0f};

RaytraceRenderWidget::RaytraceRenderWidget(
    std::vector<ThreeDModel>* newTexturedObject,
    RenderParameters* newRenderParameters,
    QWidget* parent)
    : QOpenGLWidget(parent),
      texturedObjects(newTexturedObject),
      renderParameters(newRenderParameters),
      scene(newTexturedObject, newRenderParameters),
      modelView(glm::identity<glm::mat4>()) {
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RaytraceRenderWidget::forceRepaint);
    timer->start(30);
}

// empty (for now)
// all of our pointers are to data owned by another class
// so we have no responsibility for destruction
// and OpenGL cleanup is taken care of by Qt
RaytraceRenderWidget::~RaytraceRenderWidget() = default;

// mouse-handling
void RaytraceRenderWidget::mousePressEvent(QMouseEvent* event) {
    // RaytraceRenderWidget::mousePressEvent()
    // store the button for future reference
    int whichButton = event->button();
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width
    float size = widgetWidth() > widgetHeight() ? widgetHeight() : widgetWidth();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y()) / size;

    // and we want to force mouse buttons to allow shift-click to be the same as right-click
    unsigned int modifiers = event->modifiers();

    // shift-click (any) counts as right click
    if (modifiers & Qt::ShiftModifier)
        whichButton = Qt::RightButton;

    // send signal to the controller for detailed processing
    emit beginScaledDrag(whichButton, x, y);
} // RaytraceRenderWidget::mousePressEvent()

void RaytraceRenderWidget::mouseMoveEvent(QMouseEvent* event) {
    // RaytraceRenderWidget::mouseMoveEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width
    float size = (widgetWidth() > widgetHeight()) ? widgetHeight() : widgetWidth();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y()) / size;

    // send signal to the controller for detailed processing
    emit continueScaledDrag(x, y);
} // RaytraceRenderWidget::mouseMoveEvent()

void RaytraceRenderWidget::mouseReleaseEvent(QMouseEvent* event) {
    // RaytraceRenderWidget::mouseReleaseEvent()
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width
    float size = widgetWidth() > widgetHeight() ? widgetHeight() : widgetWidth();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y()) / size;

    // send signal to the controller for detailed processing
    emit endScaledDrag(x, y);
} // RaytraceRenderWidget::mouseReleaseEvent()


void RaytraceRenderWidget::initializeGL() {
    // this should remain empty
}

void RaytraceRenderWidget::resizeGL(int width, int height) {
    frameBuffer.resize(width, height);
}

void RaytraceRenderWidget::paintGL() {
    // set background colour to white
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // and display the image
    glDrawPixels(frameBuffer.width, frameBuffer.height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.block);
}

void RaytraceRenderWidget::forceRepaint() {
    update();
}

float RaytraceRenderWidget::widgetWidth() const {
    return static_cast<float>(frameBuffer.width);
}

float RaytraceRenderWidget::widgetHeight() const {
    return static_cast<float>(frameBuffer.height);
}

float RaytraceRenderWidget::aspectRatio() const {
    return this->widgetWidth() / this->widgetHeight();
}

bool RaytraceRenderWidget::isCorner(int x, int y) const {
    // clang-format off
    return (x == 0 && y == 0) ||
           (x == 0 && y == frameBuffer.height - 1) ||
           (x == frameBuffer.width - 1 && y == 0) ||
           (x == frameBuffer.width - 1 && y == frameBuffer.height - 1);
    // clang-format on
}

/**
 * @param pixelX location of the pixel in x-axis
 * @param pixelY location of the pixel in y-axis
 * @param aspectRatio of the widget
 *
 * @return a Ray with origin at camera and direction pointing towards the pixel (x, y)
 */
Ray RaytraceRenderWidget::rayToPixel(
    const float pixelX,
    const float pixelY,
    const float aspectRatio
) const {
    float xNdcs = (pixelX / widgetWidth() - 0.5f) * 2.0f;
    float yNdcs = (pixelY / widgetHeight() - 0.5f) * 2.0f;

    float x;
    float y;

    if (aspectRatio > 1.0f) {
        // Landscape case, x-axis is stretched
        x = xNdcs * aspectRatio;
        y = yNdcs;
    } else {
        // Portrait case, y-axis is stretched
        x = xNdcs;
        y = yNdcs / aspectRatio;
    }

    if (renderParameters->orthoProjection) {
        return Ray(
            glm::vec3(x, y, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 direction = glm::normalize(glm::vec3(
            x - camera.x,
            y - camera.y,
            // Camera plane is at (-1) given that Z+ points outside the screen
            -1.0f - camera.z)
    );

    if (isCorner(pixelX, pixelY)) {
        std::cout << std::endl;
        std::cout << "Ray to pixel -> (" << pixelX << ", " << pixelY << ")" << std::endl;

        std::cout << "Origin: " << glm::to_string(camera) << std::endl;
        std::cout << "Direction: " << glm::to_string(direction) << std::endl;
    }

    return Ray(camera, direction);
}

void RaytraceRenderWidget::Raytrace() {
    modelView = scene.modelView();

    raytracingThread = std::thread(&RaytraceRenderWidget::RaytraceMultithreaded, this);
    raytracingThread.detach();
}

std::pair<float, float> RaytraceRenderWidget::sampledPixel(const float i, const float j) const {
    const float di = static_cast<float>(random()) / static_cast<float>(RAND_MAX) - 0.5f;
    const float dj = static_cast<float>(random()) / static_cast<float>(RAND_MAX) - 0.5f;

    return {std::clamp(i + di, 0.0f, widgetWidth()), std::clamp(j + dj, 0.0f, widgetHeight())};
}

void RaytraceRenderWidget::RaytraceMultithreaded() {
    std::cout << "Start Raytracing..." << std::endl;

    scene.updateScene();

    auto aspectRatio = this->aspectRatio();
    std::cout << "Aspect Ratio: " << aspectRatio << std::endl;

    frameBuffer.clear(RGBAValue(0.0f, 0.0f, 0.0f, 1.0f));

    // clang-format off
#pragma omp parallel for schedule(dynamic)
    // clang-format on
    for (int j = 0; j < frameBuffer.height; j++) {
        for (int i = 0; i < frameBuffer.width; i++) {
            glm::vec4 colour{0.0f};
            if (renderParameters->monteCarloEnabled) {
                // Anti-aliasing
                for (unsigned int s = 0; s < N_AA_SAMPLES; s++) {
                    const auto [si, sj] = sampledPixel(i, j);
                    const Ray rayForPixel = rayToPixel(si, sj, aspectRatio);
                    colour = colour + raytraceColour(rayForPixel, airRefractiveIndex, N_BOUNCES);
                }
                colour = colour / static_cast<float>(N_AA_SAMPLES);
            } else {
                // No anti-aliasing
                const Ray rayForPixel = rayToPixel(i, j, aspectRatio);
                colour = raytraceColour(rayForPixel, airRefractiveIndex, N_BOUNCES);
            }

            // Gamma correction....
            float gamma = 2.2f;

            // We already calculate everything in float, so we just do gamma correction
            // before putting it integer format.
            colour.x = pow(colour.x, 1 / gamma);
            colour.y = pow(colour.y, 1 / gamma);
            colour.z = pow(colour.z, 1 / gamma);

            frameBuffer[j][i] = RGBAValue(
                colour.x * 255.0f,
                colour.y * 255.0f,
                colour.z * 255.0f,
                255.0f);
        }
    }

    std::cout << std::endl
            << "Done Raytracing!"
            << std::endl;
}

/**
 * @return SurfaceElement resulting from the barycentric interpolation of collisionPoint on triangle
 */
SurfaceElement barycentricInterpolation(
    const Triangle& triangle,
    const glm::vec3& collisionPoint
) {
    glm::vec3 barycentric = triangle.barycentricCoordinates(collisionPoint);

    return SurfaceElement(triangle, collisionPoint, triangle.weightedNormal(barycentric));
}

/**
 * @return Ray reflected on surface
 */
Ray reflect(const Ray& ray, const SurfaceElement& surfel) {
    // Bias origin in the direction of the normal to avoid self intersections
    glm::vec3 reflectionOrigin = surfel.point + collisionBias * surfel.normal;
    glm::vec3 reflectedDirection = ray.direction - 2.0f * glm::dot(ray.direction, surfel.normal) * surfel.normal;

    return Ray(reflectionOrigin, reflectedDirection);
}

/**
 * @return Ray refracted over surface, reflected in case of total internal reflection
 */
Ray refract(const Ray& ray, float mediumRefractiveIndex, const SurfaceElement& surfel) {
    float n1 = mediumRefractiveIndex;
    float n2 = surfel.indexOfRefraction();

    float cosTheta1 = -glm::dot(ray.direction, surfel.normal);

    // Ray is coming out of the object, it was already refracted
    if (cosTheta1 < 0.0f) {
        return Ray(surfel.point + collisionBias * surfel.normal, ray.direction);
    }

    // Bias origin to avoid refraction self intersection
    glm::vec3 refractionOrigin = surfel.point - collisionBias * surfel.normal;

    float n = n1 / n2;

    float sinTheta2Squared = n * n * (1.0f - cosTheta1 * cosTheta1);

    // Total internal reflection => reflect
    // Note: x^2 in [0..1] => x in [0..1]
    if (sinTheta2Squared > 1.0f) {
        return reflect(ray, surfel);
    }

    float cosTheta2 = std::sqrt(1.0f - sinTheta2Squared);
    glm::vec3 refractedDirection = n * ray.direction + (n * cosTheta1 - cosTheta2) * surfel.normal;

    return Ray(refractionOrigin, refractedDirection);
}

/**
 * @return the raytraced colour, capped by a maximum number of bounces on reflective surfaces
 */
glm::vec4 RaytraceRenderWidget::raytraceColour(
    const Ray& ray,
    float refractiveIndex,
    int bounces
) const {
    return traceColour(ray, refractiveIndex, bounces, true);
}

/**
 * @return the pathtraced colour, capped by a maximum number of bounces on reflective surfaces
 */
glm::vec4 RaytraceRenderWidget::pathtraceColour(const Ray& ray, float refractiveIndex, int bounces) const {
    return traceColour(ray, refractiveIndex, bounces, false);
}

/**
 * @return if Fresnel rendering is enabled, the Schlick's approximation reflectance of a surface
 *         otherwise, the reflectivity of the triangle, or 1.0 if the triangle has no transparency
 *         the result is guaranteed to be in [0..1], therefore transmittance = 1 - reflectance
 */
float RaytraceRenderWidget::reflectance(
    const Ray& ray,
    const SurfaceElement& surfel,
    const float mediumRefractiveIndex
) const {
    if (renderParameters->fresnelRendering) {
        return surfel.schlick(ray, mediumRefractiveIndex);
    }

    float reflectivity = surfel.triangle.sharedMaterial->transparency > 0.0f
                             ? surfel.triangle.sharedMaterial->reflectivity
                             : 1.0f;
    return reflectivity;
}

/**
 * @return the traced colour, capped by a maximum number of bounces on reflective surfaces
 *         if isPrimaryRay then account for all contributions, otherwise just direct lighting contributions
 */
glm::vec4 RaytraceRenderWidget::traceColour(
    const Ray& ray,
    float refractiveIndex,
    int bounces,
    bool isPrimaryRay
) const {
    if (bounces <= 0) {
        return NoColour;
    }

    CollisionInfo collision = scene.closestTriangle(ray);

    if (!collision.isHit()) {
        return NoColour;
    }

    SurfaceElement surfel = barycentricInterpolation(collision.triangle, ray.origin + collision.t * ray.direction);

    if (renderParameters->interpolationRendering) {
        return {std::abs(surfel.normal.x), std::abs(surfel.normal.y), std::abs(surfel.normal.z), 1.0f};
    }

    // Reflect + Refract colour
    if (!surfel.isPhong()) {
        auto colour = NoColour;

        const float reflectivity = reflectance(ray, surfel, refractiveIndex);
        const float refractivity = 1.0f - reflectivity;

        // Add reflection colour contribution, if needed
        if (reflectivity > 0.0f) {
            const Ray reflectionRay = reflect(ray, surfel);
            const auto& reflection = traceColour(reflectionRay, refractiveIndex, bounces - 1, isPrimaryRay);
            colour = colour + reflectivity * reflection;
        }

        // Add refraction colour contribution, if needed
        if (refractivity > 0.0f) {
            const Ray refractionRay = refract(ray, refractiveIndex, surfel);
            const auto& refraction = traceColour(refractionRay, surfel.indexOfRefraction(), bounces - 1, isPrimaryRay);
            colour = colour + refractivity * refraction;
        }

        return colour;
    }

    // Only direct lighting contribution for secondary rays
    if (!isPrimaryRay) {
        return directLightingColour(surfel, ray.origin);
    }

    // Colour of all contributions for primary rays
    return surfaceColour(surfel, ray.origin);
}

/**
 * @return the blinn-phong colour resulting of all contributions on the surface
 */
glm::vec4 RaytraceRenderWidget::surfaceColour(
    const SurfaceElement& surfel,
    const glm::vec3& eye
) const {
    auto colour = directLightingColour(surfel, eye);
    glm::vec3 biasedOrigin = surfel.point + collisionBias * surfel.normal;

    // Emission is independent of shadow, compute contribution
    colour = colour + surfel.emissive();

    // Compute indirect lighting contribution
    if (renderParameters->monteCarloEnabled) {
        auto indirectLightingColour = NoColour;
        for (unsigned int i = 0; i < N_MC_SAMPLES; i++) {
            Ray monteCarloRay = Ray(biasedOrigin, randomMonteCarloDirection(surfel.normal));
            indirectLightingColour = indirectLightingColour + pathtraceColour(
                                         monteCarloRay, surfel.indexOfRefraction(), N_BOUNCES);
        }
        float distributionFactor = N_MC_SAMPLES / 2 * M_PI;
        indirectLightingColour = indirectLightingColour / distributionFactor;
        colour = colour + indirectLightingColour;
    } else {
        colour = colour + surfel.indirectLighting();
    }

    return {
        std::clamp(colour.r, 0.0f, 1.0f),
        std::clamp(colour.g, 0.0f, 1.0f),
        std::clamp(colour.b, 0.0f, 1.0f),
        std::clamp(colour.a, 0.0f, 1.0f)
    };
}

/**
 * @return the blinn-phong colour resulting only from direct lighting contributions on the surface
 */
glm::vec4 RaytraceRenderWidget::directLightingColour(
    const SurfaceElement& surfel,
    const glm::vec3& eye
) const {
    // Bias the origin in the direction of the normal to avoid issues with self-intersection
    // E.g.: Shadow acne
    glm::vec3 biasedPoint = surfel.point + collisionBias * surfel.normal;
    auto colour = NoColour;

    for (const auto& light : renderParameters->lights) {
        if (!light->enabled) {
            continue;
        }

        // Convert light position to view coordinates
        glm::vec4 lightPosition = modelView * light->lightPosition;
        glm::vec4 lightColour = light->lightColor;

        auto directColour = surfel.directLighting(lightPosition, lightColour, {eye, 1.0f});

        if (renderParameters->shadowsEnabled) {
            directColour = directColour * shadowModulation(biasedPoint, light);
        }

        colour = colour + directColour;
    }

    return {
        std::clamp(colour.r, 0.0f, 1.0f),
        std::clamp(colour.g, 0.0f, 1.0f),
        std::clamp(colour.b, 0.0f, 1.0f),
        std::clamp(colour.a, 0.0f, 1.0f)
    };
}

/**
 * @return the modulation factors of the shadow of a light on a point
 *         guaranteed to be of the form (sf, sf, sf, 1), to only alter (r, g, b)
 */
glm::vec4 RaytraceRenderWidget::shadowModulation(const glm::vec3& point, const Light* light) const {
    float shadowFactor;

    if (renderParameters->areaLightsEnabled) {
        // Soft shadows
        unsigned int hits = 0;

        for (unsigned int i = 0; i < N_SS_SAMPLES; i++) {
            const glm::vec3 lightPosition = modelView * light->sampledPosition();

            hits += isShadowHit(lightPosition, point) ? 1 : 0;
        }

        // shadowFactor = 1 - % of shadow hits = % of non-shadow hits
        shadowFactor = 1.0f - hits / static_cast<float>(N_SS_SAMPLES);
    } else {
        // Sharp shadows
        glm::vec3 lightPosition = modelView * light->lightPosition;
        // shadowFactor = either full or no shadow
        shadowFactor = isShadowHit(lightPosition, point) ? 0.0f : 1.0f;
    }

    return glm::vec4(shadowFactor, shadowFactor, shadowFactor, 1.0f);
}

bool RaytraceRenderWidget::isShadowHit(const glm::vec3& lightPosition, const glm::vec3& point) const {
    Ray shadowRay(point, glm::normalize(lightPosition - point));
    SurfaceElement* surfel = nullptr;
    float refractiveIndex = airRefractiveIndex;

    // Follow ray along refractions until shadow hit is confirmed or ray is exhausted
    do {
        const CollisionInfo collision = scene.closestTriangle(shadowRay);

        if (!collision.isShadowHit()) {
            break;
        }

        SurfaceElement shadowSurfel = barycentricInterpolation(collision.triangle,
                                                               shadowRay.origin + collision.t * shadowRay.direction);

        float refractivity = 1.0f - reflectance(shadowRay, shadowSurfel, refractiveIndex);
        surfel = &shadowSurfel;

        if (refractivity > 0.0f) {
            shadowRay = refract(shadowRay, refractiveIndex, shadowSurfel);
            refractiveIndex = shadowSurfel.indexOfRefraction();
        } else {
            break;
        }
    } while (surfel != nullptr && !surfel->isPhong());

    return surfel != nullptr && surfel->isPhong() && !surfel->triangle.sharedMaterial->isLight();
}
