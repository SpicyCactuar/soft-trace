#ifndef ARCBALL_WIDGET_H
#define ARCBALL_WIDGET_H

#include <QMouseEvent>
#include <QOpenGLWidget>

#include "ArcBall.h"

class ArcBallWidget : public QOpenGLWidget {
    Q_OBJECT

private:
    ArcBall ball;

public:
    ArcBallWidget(QWidget* parent);

    glm::mat4 rotationMatrix() const;

protected:
    void initializeGL();

    void resizeGL(int width, int height);

    void paintGL();

    // mouse-handling
    virtual void mousePressEvent(QMouseEvent* event);

    virtual void mouseMoveEvent(QMouseEvent* event);

    virtual void mouseReleaseEvent(QMouseEvent* event);

public slots:
    // routines called to allow synchronised rotation with other widget
    // coordinates are assumed to be in range of [-1..1] in x,y
    // if outside that range, will be clamped
    void beginDrag(float x, float y);

    void continueDrag(float x, float y);

    void endDrag(float x, float y);

signals:
    // slot for calling controller when rotation changes
    void rotationChanged();
};

#endif
