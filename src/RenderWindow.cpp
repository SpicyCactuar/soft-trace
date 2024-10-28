#include "RenderWindow.h"

#include "RenderParameters.h"

RenderWindow::RenderWindow(
    std::vector<ThreeDModel>* newTexturedObject,
    RenderParameters* newRenderParameters,
    const char* windowName)
    : QWidget(nullptr), // widget has no parent
      texturedObject(newTexturedObject),
      renderParameters(newRenderParameters) {
    // set the window's title
    setWindowTitle(QString(windowName));

    // initialise the grid layout
    windowLayout = new QGridLayout(this);

    // create all of the widgets, starting with the custom render widgets
    renderWidget = new RenderWidget(newTexturedObject, newRenderParameters, this);
    raytraceRenderWidget = new RaytraceRenderWidget(newTexturedObject, newRenderParameters, this);

    // construct custom arcball Widgets
    modelRotator = new ArcBallWidget(this);

    // construct standard QT widgets
    // check boxes
    interpolationBox = new QCheckBox("Interpolation", this);
    shadowBox = new QCheckBox("Shadow", this);
    fresnelRenderingBox = new QCheckBox("Fresnel", this);
    monteCarloBox = new QCheckBox("Monte-Carlo", this);
    areaLightsBox = new QCheckBox("Area Lights", this);
    orthographicBox = new QCheckBox("Orthographic", this);

    // buttons
    raytraceButton = new QPushButton("Raytrace", this);

    // spatial sliders
    xTranslateSlider = new QSlider(Qt::Horizontal, this);
    secondXTranslateSlider = new QSlider(Qt::Horizontal, this);
    yTranslateSlider = new QSlider(Qt::Vertical, this);
    zTranslateSlider = new QSlider(Qt::Vertical, this);

    // labels for sliders and arcballs
    modelRotatorLabel = new QLabel("Model", this);
    yTranslateLabel = new QLabel("Y", this);
    zoomLabel = new QLabel("Zm", this);

    // add all of the widgets to the grid Row --- Column --- Row Span --- Column Span

    // the top two widgets have to fit to the widgets stack between them
    int nStacked = 13;

    windowLayout->addWidget(renderWidget, 0, 1, nStacked, 1);
    windowLayout->addWidget(yTranslateSlider, 0, 2, nStacked, 1);
    windowLayout->addWidget(zTranslateSlider, 0, 4, nStacked, 1);
    windowLayout->addWidget(raytraceRenderWidget, 0, 5, nStacked, 1);

    // the stack in the middle
    windowLayout->addWidget(modelRotator, 0, 3, 1, 1);
    windowLayout->addWidget(modelRotatorLabel, 1, 3, 1, 1);
    windowLayout->addWidget(interpolationBox, 2, 3, 1, 1);
    windowLayout->addWidget(shadowBox, 3, 3, 1, 1);
    windowLayout->addWidget(fresnelRenderingBox, 4, 3, 1, 1);
    windowLayout->addWidget(monteCarloBox, 5, 3, 1, 1);
    windowLayout->addWidget(areaLightsBox, 6, 3, 1, 1);
    windowLayout->addWidget(orthographicBox, 7, 3, 1, 1);

    // Raytrace Button
    windowLayout->addWidget(raytraceButton, 0, 6, nStacked, 1);

    // Translate Slider Row
    windowLayout->addWidget(xTranslateSlider, nStacked, 1, 1, 1);
    windowLayout->addWidget(yTranslateLabel, nStacked, 2, 1, 1);
    // nothing in column 3
    windowLayout->addWidget(zoomLabel, nStacked, 4, 1, 1);
    windowLayout->addWidget(secondXTranslateSlider, nStacked, 5, 1, 1);

    // now reset all of the control elements to match the render parameters passed in
    resetInterface();
}

// sets every visual control to match the model
// gets called by the controller after each change in the model
void RenderWindow::resetInterface() const {
    // set check boxes
    interpolationBox->setChecked(renderParameters->interpolationRendering);
    shadowBox->setChecked(renderParameters->shadowsEnabled);
    fresnelRenderingBox->setChecked(renderParameters->fresnelRendering);
    monteCarloBox->setChecked(renderParameters->monteCarloEnabled);
    areaLightsBox->setChecked(renderParameters->areaLightsEnabled);
    orthographicBox->setChecked(renderParameters->orthoProjection);

    // set sliders
    // x & y translate are scaled to notional unit sphere in render widgets
    // but because the slider is defined as integer, we multiply by a 100 for all sliders
    xTranslateSlider->setMinimum(int(TRANSLATE_MIN * PARAMETER_SCALING));
    xTranslateSlider->setMaximum(int(TRANSLATE_MAX * PARAMETER_SCALING));
    xTranslateSlider->setValue(int(renderParameters->xTranslate * PARAMETER_SCALING));

    secondXTranslateSlider->setMinimum(int(TRANSLATE_MIN * PARAMETER_SCALING));
    secondXTranslateSlider->setMaximum(int(TRANSLATE_MAX * PARAMETER_SCALING));
    secondXTranslateSlider->setValue(int(renderParameters->xTranslate * PARAMETER_SCALING));

    yTranslateSlider->setMinimum(int(TRANSLATE_MIN * PARAMETER_SCALING));
    yTranslateSlider->setMaximum(int(TRANSLATE_MAX * PARAMETER_SCALING));
    yTranslateSlider->setValue(int(renderParameters->yTranslate * PARAMETER_SCALING));

    // zoom slider is a logarithmic scale, so we want a narrow range
    zTranslateSlider->setMinimum(int(TRANSLATE_MIN * PARAMETER_SCALING));
    zTranslateSlider->setMaximum(int(TRANSLATE_MAX * PARAMETER_SCALING));
    zTranslateSlider->setValue(int(renderParameters->zTranslate * PARAMETER_SCALING));

    // main lighting parameters are simple 0.0-1.0

    // now flag them all for update
    renderWidget->update();
    raytraceRenderWidget->update();
    modelRotator->update();
    xTranslateSlider->update();
    secondXTranslateSlider->update();
    yTranslateSlider->update();
    zTranslateSlider->update();
    interpolationBox->update();
    shadowBox->update();
    fresnelRenderingBox->update();
    monteCarloBox->update();
    areaLightsBox->update();
    orthographicBox->update();
}

void RenderWindow::handleRaytrace() const {
    raytraceRenderWidget->Raytrace();
}
