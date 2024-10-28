#ifndef RAYTRACE_RENDER_WIDGET_H
#define RAYTRACE_RENDER_WIDGET_H

#include <mutex>
#include <vector>
#include <thread>

#include <QMouseEvent>
#include <QOpenGLWidget>

#include "Ray.h"
#include "RenderParameters.h"
#include "Scene.h"
#include "SurfaceElement.h"
#include "ThreeDModel.h"

// Render widget with arcball linked to an arcball widget
class RaytraceRenderWidget : public QOpenGLWidget {
    Q_OBJECT

private:
    std::vector<ThreeDModel>* texturedObjects;

    RenderParameters* renderParameters;

    RGBAImage frameBuffer;

    // Separate thread to trigger re-rendering
    std::thread raytracingThread;

    Scene scene;

    void forceRepaint();

    void RaytraceMultithreaded();

public:
    // constructor
    RaytraceRenderWidget(
        // the geometric object to show
        std::vector<ThreeDModel>* newTexturedObject,
        // the render parameters to use
        RenderParameters* newRenderParameters,
        // parent widget in visual hierarchy
        QWidget* parent);

    void Raytrace();

    // destructor
    ~RaytraceRenderWidget();

protected:
    // called when OpenGL context is set up
    void initializeGL();

    // called every time the widget is resized
    void resizeGL(int width, int height);

    // called every time the widget needs painting
    void paintGL();

    // mouse-handling
    virtual void mousePressEvent(QMouseEvent* event);

    virtual void mouseMoveEvent(QMouseEvent* event);

    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    glm::mat4 modelView;

    float aspectRatio() const;

    float widgetWidth() const;

    float widgetHeight() const;

    bool isCorner(int x, int y) const;

    std::pair<float, float> sampledPixel(float i, float j) const;

    Ray rayToPixel(float pixelX, float pixelY, float aspectRatio) const;

    glm::vec4 raytraceColour(const Ray& ray, float refractiveIndex, int bounces) const;

    glm::vec4 pathtraceColour(const Ray& ray, float refractiveIndex, int bounces) const;

    glm::vec4 traceColour(const Ray& ray, float refractiveIndex, int bounces, bool isPrimaryRay) const;

    glm::vec4 surfaceColour(const SurfaceElement& surfel, const glm::vec3& eye) const;

    glm::vec4 directLightingColour(const SurfaceElement& surfel, const glm::vec3& eye) const;

    glm::vec4 shadowModulation(const glm::vec3& point, const Light* light) const;

    bool isShadowHit(const glm::vec3& lightPosition, const glm::vec3& point) const;

    float reflectance(const Ray& ray, const SurfaceElement& surfel, float mediumRefractiveIndex) const;

signals:
    // these are general purpose signals, which scale the drag to
    // the notional unit sphere and pass it to the controller for handling
    void beginScaledDrag(int whichButton, float x, float y);

    // note that Continue & End assume the button has already been set
    void continueScaledDrag(float x, float y);

    void endScaledDrag(float x, float y);
};

#endif
