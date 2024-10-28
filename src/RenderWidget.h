#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <vector>

#include <QMouseEvent>
#include <QOpenGLWidget>

#include "RenderParameters.h"
#include "ThreeDModel.h"

// Render widget with arcball linked to an arcball widget
class RenderWidget : public QOpenGLWidget {
    Q_OBJECT

private:
    std::vector<ThreeDModel>* texturedObjects;

    RenderParameters* renderParameters;

    void renderScene() const;

    void lightScene() const;

    void paintScene() const;

public:
    // constructor
    RenderWidget(
        std::vector<ThreeDModel>* newTexturedObject,
        RenderParameters* newRenderParameters,
        QWidget* parent);

    // destructor
    ~RenderWidget();

protected:
    void initializeGL();

    void resizeGL(int width, int height);

    void paintGL();

    // mouse-handling
    virtual void mousePressEvent(QMouseEvent* event);

    virtual void mouseMoveEvent(QMouseEvent* event);

    virtual void mouseReleaseEvent(QMouseEvent* event);

    // these signals are needed to support shared arcball control
signals:
    // these are general purpose signals, which scale the drag to
    // the notional unit sphere and pass it to the controller for handling
    void beginScaledDrag(int whichButton, float x, float y);

    // note that Continue & End assume the button has already been set
    void continueScaledDrag(float x, float y);

    void endScaledDrag(float x, float y);
};

#endif
