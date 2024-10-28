#include "ArcBallWidget.h"

#include <glm/gtc/type_ptr.hpp>

#define ARCBALL_WIDGET_SIZE 100

static constexpr std::array<glm::vec3, 84> sphereVert {
    glm::vec3{0, 0, -1},
    glm::vec3{0.5f, 0, -0.86603f},
    glm::vec3{0.86603f, 0, -0.5f},
    glm::vec3{1, 0, 0},
    glm::vec3{0.86603f, 0, 0.5f},
    glm::vec3{0.5f, 0, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{0.43301f, 0.25f, -0.86603f},
    glm::vec3{0.75f, 0.43301f, -0.5f},
    glm::vec3{0.86603f, 0.5f, 0},
    glm::vec3{0.75f, 0.43301f, 0.5f},
    glm::vec3{0.43301f, 0.25f, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{0.25f, 0.43301f, -0.86603f},
    glm::vec3{0.43301f, 0.75f, -0.5f},
    glm::vec3{0.5f, 0.86603f, 0},
    glm::vec3{0.43301f, 0.75f, 0.5f},
    glm::vec3{0.25f, 0.43301f, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{0, 0.5f, -0.86603f},
    glm::vec3{0, 0.86603f, -0.5f},
    glm::vec3{0, 1, 0},
    glm::vec3{0, 0.86603f, 0.5f},
    glm::vec3{0, 0.5f, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{-0.25f, 0.43301f, -0.86603f},
    glm::vec3{-0.43301f, 0.75f, -0.5f},
    glm::vec3{-0.5f, 0.86603f, 0},
    glm::vec3{-0.43301f, 0.75f, 0.5f},
    glm::vec3{-0.25f, 0.43301f, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{-0.43301f, 0.25f, -0.86603f},
    glm::vec3{-0.75f, 0.43301f, -0.5f},
    glm::vec3{-0.86603f, 0.5f, 0},
    glm::vec3{-0.75f, 0.43301f, 0.5f},
    glm::vec3{-0.43301f, 0.25f, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{-0.5f, 0, -0.86603f},
    glm::vec3{-0.86603f, 0, -0.5f},
    glm::vec3{-1, 0, 0},
    glm::vec3{-0.86603f, 0, 0.5f},
    glm::vec3{-0.5f, 0, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{-0.43301f, -0.25f, -0.86603f},
    glm::vec3{-0.75f, -0.43301f, -0.5f},
    glm::vec3{-0.86603f, -0.5f, 0},
    glm::vec3{-0.75f, -0.43301f, 0.5f},
    glm::vec3{-0.43301f, -0.25f, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{-0.25f, -0.43301f, -0.86603f},
    glm::vec3{-0.43301f, -0.75f, -0.5f},
    glm::vec3{-0.5f, -0.86603f, 0},
    glm::vec3{-0.43301f, -0.75f, 0.5f},
    glm::vec3{-0.25f, -0.43301f, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{0, -0.5f, -0.86603f},
    glm::vec3{0, -0.86603f, -0.5f},
    glm::vec3{0, -1, 0},
    glm::vec3{0, -0.86603f, 0.5f},
    glm::vec3{0, -0.5f, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{0.25f, -0.43301f, -0.86603f},
    glm::vec3{0.43301f, -0.75f, -0.5f},
    glm::vec3{0.5f, -0.86603f, 0},
    glm::vec3{0.43301f, -0.75f, 0.5f},
    glm::vec3{0.25f, -0.43301f, 0.86603f},
    glm::vec3{0, 0, 1},
    glm::vec3{0, 0, -1},
    glm::vec3{0.43301f, -0.25f, -0.86603f},
    glm::vec3{0.75f, -0.43301f, -0.5f},
    glm::vec3{0.86603f, -0.5f, 0},
    glm::vec3{0.75f, -0.43301f, 0.5f},
    glm::vec3{0.43301f, -0.25f, 0.86603f},
    glm::vec3{0, 0, 1}
};

ArcBallWidget::ArcBallWidget(QWidget* parent) : QOpenGLWidget(parent) {
    // let QT know we are fixed size
    setFixedSize(QSize(ARCBALL_WIDGET_SIZE, ARCBALL_WIDGET_SIZE));
}

glm::mat4 ArcBallWidget::rotationMatrix() const {
    return ball.rotationMatrix();
}

void ArcBallWidget::initializeGL() {
    // no lighting, but we need depth test
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    // background is white
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
}

void ArcBallWidget::resizeGL(int width, int height) {
    // reset the viewport
    glViewport(0, 0, width, height);

    // set projection matrix to have range of -1.0 - 1.0 in x, y, z
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

// called every time the widget needs painting
void ArcBallWidget::paintGL() {
    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set model view matrix based on arcball rotation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw a single white quad to block back half of arcball
    glColor3f(0.92f, 0.92f, 0.92f);
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();

    // set colour to black
    glColor3f(0.0f, 0.0f, 0.0f);
    // draw a circle around the edge of the sphere
    // reusing data from some of the elements of the sphere
    glScalef(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINE_LOOP);
    for (int j = 3; j < 84; j += 7) {
        glVertex3f(sphereVert[j][0], sphereVert[j][1], 0.5);
    }
    glEnd();

    // retrieve rotation from arcball & apply
    glm::mat4 rotMatrix = ball.rotationMatrix();
    glMultMatrixf(glm::value_ptr(rotMatrix));

    // loop through verticals of sphere
    for (int i = 0; i < 12; i++) {
        // loop drawing verticals
        glBegin(GL_LINE_STRIP);
        for (int j = i * 7; j < 7 + i * 7; j++) {
            glVertex3fv(glm::value_ptr(sphereVert[j]));
        }
        glEnd();
    }

    // and loop through horizontals
    for (int i = 1; i < 6; i++) {
        // loop for horizontals
        glBegin(GL_LINE_LOOP);
        for (int j = i; j < 84; j += 7) {
            glVertex3fv(glm::value_ptr(sphereVert[j]));
        }
        glEnd();
    }
}

// mouse-handling
void ArcBallWidget::mousePressEvent(QMouseEvent* event) {
    float x = event->x();
    float y = event->y();
    float width = this->width();
    float height = this->height();
    float scaledX = (2.0f * x - width) / width;
    // this one has to flip from Qt coordinates to Cartesian
    float scaledY = (height - 2.0f * y) / height;
    // set the initial rotation for the drag
    ball.beginDrag(scaledX, scaledY);
    // and send a signal that we'ver changed
    emit rotationChanged();
}

void ArcBallWidget::mouseMoveEvent(QMouseEvent* event) {
    float x = event->x();
    float y = event->y();
    float width = this->width();
    float height = this->height();
    float scaledX = (2.0f * x - width) / width;
    // this one has to flip from Qt coordinates to Cartesian
    float scaledY = (height - 2.0f * y) / height;
    // set the mid-point of the drag
    ball.continueDrag(scaledX, scaledY);
    // and send a signal that we'ver changed
    emit rotationChanged();
}

void ArcBallWidget::mouseReleaseEvent(QMouseEvent* event) {
    float x = event->x();
    float y = event->y();
    float width = this->width();
    float height = this->height();
    float scaledX = (2.0f * x - width) / width;
    // this one has to flip from Qt coordinates to Cartesian
    float scaledY = (height - 2.0f * y) / height;
    // set the final rotation for the drag
    ball.endDrag(scaledX, scaledY);
    // and send a signal that we'ver changed
    emit rotationChanged();
}

// routines called to allow synchronised rotation with other widget
// coordinates are assumed to be in range of [-1..1] in x,y
// if outside that range, will be clamped
void ArcBallWidget::beginDrag(float x, float y) {
    ball.beginDrag(x, y);
    emit rotationChanged();
}

void ArcBallWidget::continueDrag(float x, float y) {
    ball.continueDrag(x, y);
    emit rotationChanged();
}

void ArcBallWidget::endDrag(float x, float y) {
    ball.endDrag(x, y);
    emit rotationChanged();
}
