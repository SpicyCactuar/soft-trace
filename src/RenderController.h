#ifndef RENDER_CONTROLLER_H
#define RENDER_CONTROLLER_H

#include <vector>
#include <QtGui>

#include "RenderParameters.h"
#include "RenderWindow.h"

class RenderController : public QObject {
    Q_OBJECT

private:
    RenderParameters* renderParameters;

    RenderWindow* renderWindow;

    int dragButton;

public:
    // constructor
    RenderController(
        // the render parameters to use
        RenderParameters* newRenderParameters,
        // the render window that it controls
        RenderWindow* newRenderWindow);

public slots:
    // slot for responding to arcball rotation for object
    void objectRotationChanged() const;

    // slots for responding to zoom & translate sliders
    void xTranslateChanged(int value) const;

    void yTranslateChanged(int value) const;

    void zTranslateChanged(int value) const;

    // slots for responding to check boxes
    void interpolationCheckChanged(int state) const;

    void shadowBoxCheckChanged(int state) const;

    void fresnelRenderingCheckChanged(int state) const;

    void monteCarloBoxChanged(int state) const;

    void areaLightsBoxChanged(int state) const;

    void orthographicBoxChanged(int state) const;

    void raytraceCalled() const;

    // slots for responding to arcball manipulations
    // these are general purpose signals which pass the mouse moves to the controller
    // after scaling to the notional unit sphere
    void beginScaledDrag(int whichButton, float x, float y);

    // note that Continue & End assume the button has already been set
    void continueScaledDrag(float x, float y) const;

    void endScaledDrag(float x, float y);
};

// end of include guard
#endif
