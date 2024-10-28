#include "RenderWidget.h"

#include <cmath>
#include <gtc/type_ptr.hpp>

// constructor
RenderWidget::RenderWidget(
    std::vector<ThreeDModel>* newTexturedObject,
    RenderParameters* newRenderParameters,
    QWidget* parent)
    : QOpenGLWidget(parent),
      texturedObjects(newTexturedObject),
      renderParameters(newRenderParameters) {
}

RenderWidget::~RenderWidget() {
    // all of our pointers are to data owned by another class
    // so we have no responsibility for destruction
    // and OpenGL cleanup is taken care of by Qt
}

void RenderWidget::initializeGL() {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

    float globalAmbientColour[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColour);

    // background is black
    glClearColor(0, 0, 0, 1.0);
}

void RenderWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);

    // set projection matrix to be glOrtho based on zoom & window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // compute the aspect ratio of the widget
    double aspectRatio = static_cast<double>(width) / static_cast<double>(height);

    // we want to capture a sphere of radius 1.0 without distortion
    // so we set the ortho projection based on whether the window is portrait (> 1.0) or landscape
    // portrait ratio is wider, so make bottom & top -1.0 & 1.0
    if (aspectRatio > 1.0) {
        if (renderParameters->orthoProjection) {
            glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, 0.0, 2.0);
        } else {
            glFrustum(-aspectRatio * 0.01, aspectRatio * 0.01, -0.01, 0.01, 0.01, 200.0);
        }
    } else {
        if (renderParameters->orthoProjection) {
            glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, 0, 2.0);
        } else {
            glFrustum(-0.01, 0.01, -0.01 / aspectRatio, 0.01 / aspectRatio, 0.01, 200.0);
        }
    }
}

void RenderWidget::paintGL() {
    // Resize to re-set the projection
    // Not the most performant solution
    resizeGL(width(), height());

    // enable depth-buffering
    glEnable(GL_DEPTH_TEST);
    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set model view matrix based on stored translation, rotation &c.

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);

    // translate by the visual translation
    // Lets center the scene on -1 so we can have a camera with near 0 and far 2
    // without having to do many changes
    glTranslatef(renderParameters->xTranslate, renderParameters->yTranslate, renderParameters->zTranslate - 1);

    // apply rotation matrix from arcball
    glMultMatrixf(glm::value_ptr(renderParameters->rotationMatrix));

    // called every time the widget needs painting
    paintScene();
}

void RenderWidget::paintScene() const {
    // Order is important: First light and then render scene
    lightScene();
    renderScene();
}

void RenderWidget::lightScene() const {
    for (unsigned int i = 0; i < renderParameters->lights.size(); i++) {
        glEnable(GL_LIGHT0 + i);

        Light* currentLight = renderParameters->lights[i];

        glLightfv(GL_LIGHT0 + i, GL_POSITION, glm::value_ptr(currentLight->lightPosition));

        glLightfv(GL_LIGHT0 + i, GL_AMBIENT, glm::value_ptr(currentLight->lightColor));
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, glm::value_ptr(currentLight->lightColor));
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, glm::value_ptr(currentLight->lightColor));

        glLightf(GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, 0.0f);

        glLightf(GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, 0.0f);

        glLightf(GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, 1.0f);
    }
}

void RenderWidget::renderScene() const {
    for (const auto& texturedObject : *texturedObjects) {
        texturedObject.render();
    }
}

// mouse-handling
void RenderWidget::mousePressEvent(QMouseEvent* event) {
    // store the button for future reference
    int whichButton = event->button();
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width
    float size = width() > height() ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y()) / size;

    // and we want to force mouse buttons to allow shift-click to be the same as right-click
    unsigned int modifiers = event->modifiers();

    // shift-click (any) counts as right click
    if (modifiers & Qt::ShiftModifier) {
        whichButton = Qt::RightButton;
    }

    // send signal to the controller for detailed processing
    emit beginScaledDrag(whichButton, x, y);
}

void RenderWidget::mouseMoveEvent(QMouseEvent* event) {
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width
    float size = (width() > height()) ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y()) / size;

    // send signal to the controller for detailed processing
    emit continueScaledDrag(x, y);
}

void RenderWidget::mouseReleaseEvent(QMouseEvent* event) {
    // scale the event to the nominal unit sphere in the widget:
    // find the minimum of height & width
    float size = width() > height() ? height() : width();
    // scale both coordinates from that
    float x = (2.0f * event->x() - size) / size;
    float y = (size - 2.0f * event->y()) / size;

    // send signal to the controller for detailed processing
    emit endScaledDrag(x, y);
}
