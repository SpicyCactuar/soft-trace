#include "RenderController.h"

RenderController::RenderController(
    RenderParameters* newRenderParameters,
    RenderWindow* newRenderWindow)
    : renderParameters(newRenderParameters),
      renderWindow(newRenderWindow),
      dragButton(Qt::NoButton) {
    // signals for arcballs
    QObject::connect(renderWindow->modelRotator,
                     SIGNAL(rotationChanged()),
                     this,
                     SLOT(objectRotationChanged()));

    // signals for main widget to control arcball
    QObject::connect(renderWindow->renderWidget,
                     SIGNAL(beginScaledDrag(int, float, float)),
                     this,
                     SLOT(beginScaledDrag(int, float, float)));
    QObject::connect(renderWindow->renderWidget,
                     SIGNAL(continueScaledDrag(float, float)),
                     this,
                     SLOT(continueScaledDrag(float, float)));
    QObject::connect(renderWindow->renderWidget,
                     SIGNAL(endScaledDrag(float, float)),
                     this,
                     SLOT(endScaledDrag(float, float)));

    // signals for raytrace widget to control arcball
    QObject::connect(renderWindow->raytraceRenderWidget,
                     SIGNAL(beginScaledDrag(int, float, float)),
                     this,
                     SLOT(beginScaledDrag(int, float, float)));
    QObject::connect(renderWindow->raytraceRenderWidget,
                     SIGNAL(continueScaledDrag(float, float)),
                     this,
                     SLOT(continueScaledDrag(float, float)));
    QObject::connect(renderWindow->raytraceRenderWidget,
                     SIGNAL(endScaledDrag(float, float)),
                     this,
                     SLOT(endScaledDrag(float, float)));

    // signal for zoom slider
    QObject::connect(renderWindow->zTranslateSlider,
                     SIGNAL(valueChanged(int)),
                     this,
                     SLOT(zTranslateChanged(int)));

    // signal for x translate sliders
    QObject::connect(renderWindow->xTranslateSlider,
                     SIGNAL(valueChanged(int)),
                     this,
                     SLOT(xTranslateChanged(int)));
    QObject::connect(renderWindow->secondXTranslateSlider,
                     SIGNAL(valueChanged(int)),
                     this,
                     SLOT(xTranslateChanged(int)));

    // signal for y translate slider
    QObject::connect(renderWindow->yTranslateSlider,
                     SIGNAL(valueChanged(int)),
                     this,
                     SLOT(yTranslateChanged(int)));

    // signal for check box
    QObject::connect(renderWindow->interpolationBox,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(interpolationCheckChanged(int)));
    QObject::connect(renderWindow->shadowBox,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(shadowBoxCheckChanged(int)));
    QObject::connect(renderWindow->fresnelRenderingBox,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(fresnelRenderingCheckChanged(int)));
    QObject::connect(renderWindow->monteCarloBox,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(monteCarloBoxChanged(int)));
    QObject::connect(renderWindow->areaLightsBox,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(areaLightsBoxChanged(int)));
    QObject::connect(renderWindow->orthographicBox,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(orthographicBoxChanged(int)));

    // Connect Raytrace Button
    QObject::connect(renderWindow->raytraceButton,
                     SIGNAL(released()),
                     this,
                     SLOT(raytraceCalled()));

    // copy the rotation matrix from the widgets to the model
    renderParameters->rotationMatrix = renderWindow->modelRotator->rotationMatrix();
}

// slot for responding to arcball rotation for object
void RenderController::objectRotationChanged() const {
    // copy the rotation matrix from the widget to the model
    renderParameters->rotationMatrix = renderWindow->modelRotator->rotationMatrix();

    renderWindow->resetInterface();
}

// slot for responding to arcball rotation for light

void RenderController::xTranslateChanged(int value) const {
    renderParameters->xTranslate = value / 100.0f;

    // clamp it
    if (renderParameters->xTranslate < TRANSLATE_MIN) {
        renderParameters->xTranslate = TRANSLATE_MIN;
    } else if (renderParameters->xTranslate > TRANSLATE_MAX) {
        renderParameters->xTranslate = TRANSLATE_MAX;
    }

    renderWindow->resetInterface();
}

// slot for responding to y translate slider
void RenderController::yTranslateChanged(int value) const {
    renderParameters->yTranslate = value / 100.0f;

    // clamp it
    if (renderParameters->yTranslate < TRANSLATE_MIN) {
        renderParameters->yTranslate = TRANSLATE_MIN;
    } else if (renderParameters->yTranslate > TRANSLATE_MAX) {
        renderParameters->yTranslate = TRANSLATE_MAX;
    }

    renderWindow->resetInterface();
}

// slot for responding to z translate sliders
void RenderController::zTranslateChanged(int value) const {
    renderParameters->zTranslate = value / 100.0f;

    // clamp it
    if (renderParameters->zTranslate < TRANSLATE_MIN) {
        renderParameters->zTranslate = TRANSLATE_MIN;
    } else if (renderParameters->zTranslate > TRANSLATE_MAX) {
        renderParameters->zTranslate = TRANSLATE_MAX;
    }

    renderWindow->resetInterface();
}

void RenderController::interpolationCheckChanged(int state) const {
    renderParameters->interpolationRendering = (state == Qt::Checked);
    renderWindow->resetInterface();
}

void RenderController::shadowBoxCheckChanged(int state) const {
    renderParameters->shadowsEnabled = (state == Qt::Checked);
    renderWindow->resetInterface();
}

void RenderController::fresnelRenderingCheckChanged(int state) const {
    renderParameters->fresnelRendering = (state == Qt::Checked);
    renderWindow->resetInterface();
}

void RenderController::monteCarloBoxChanged(int state) const {
    renderParameters->monteCarloEnabled = (state == Qt::Checked);
    renderWindow->resetInterface();
}

void RenderController::areaLightsBoxChanged(int state) const {
    renderParameters->areaLightsEnabled = (state == Qt::Checked);
    renderWindow->resetInterface();
}

void RenderController::orthographicBoxChanged(int state) const {
    renderParameters->orthoProjection = (state == Qt::Checked);
    renderWindow->resetInterface();
}

void RenderController::raytraceCalled() const {
    renderWindow->handleRaytrace();
}

// slots for responding to arcball manipulations
// these are general purpose signals which pass the mouse moves to the controller
// after scaling to the notional unit sphere
void RenderController::beginScaledDrag(int whichButton, float x, float y) {
    // RenderController::beginScaledDrag()
    // depends on which button was depressed, so save that for the duration
    dragButton = whichButton;

    // now switch on it to determine behaviour
    switch (dragButton) {
        // left button drags the model
        case Qt::LeftButton:
            renderWindow->modelRotator->beginDrag(x, y);
            break;
        // middle button drags visually
        case Qt::MiddleButton:
        default:
            break;
    }

    renderWindow->resetInterface();
}

// note that Continue & End assume the button has already been set
void RenderController::continueScaledDrag(float x, float y) const {
    switch (dragButton) {
        // left button drags the model
        case Qt::LeftButton:
            renderWindow->modelRotator->continueDrag(x, y);
            break;

        // middle button drags visually
        case Qt::MiddleButton:
        default: ;
            break;
    }

    renderWindow->resetInterface();
}

void RenderController::endScaledDrag(float x, float y) {
    switch (dragButton) {
        // left button drags the model
        case Qt::LeftButton:
            renderWindow->modelRotator->endDrag(x, y);
            break;
        // middle button drags visually
        case Qt::MiddleButton:
        default:
            break;
    }

    // and reset the drag button
    dragButton = Qt::NoButton;

    renderWindow->resetInterface();
}
