#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <vector>
#include <QtWidgets>

#include "ArcBallWidget.h"
#include "RenderWidget.h"
#include "RaytraceRenderWidget.h"

// Window that displays an geometric model with controls
class RenderWindow : public QWidget {
private:
    // the geometric object being shown
    std::vector<ThreeDModel>* texturedObject;

    // the values set in the interface
    RenderParameters* renderParameters;

    // window layout
    QGridLayout* windowLayout;

    // custom widgets
    ArcBallWidget* modelRotator;
    RenderWidget* renderWidget;
    RaytraceRenderWidget* raytraceRenderWidget;

    // standard widgets
    // check boxes to control render options
    QCheckBox* interpolationBox;
    QCheckBox* shadowBox;
    QCheckBox* fresnelRenderingBox;
    QCheckBox* monteCarloBox;
    QCheckBox* areaLightsBox;
    QCheckBox* orthographicBox;

    // check boxes for modelling options
    QCheckBox* centreObjectBox;
    QCheckBox* scaleObjectBox;

    // sliders for spatial manipulation
    QSlider* xTranslateSlider;
    // we want one slider under each widget
    QSlider* secondXTranslateSlider;
    QSlider* yTranslateSlider;
    QSlider* zTranslateSlider;

    // labels for sliders & arcballs
    QLabel* modelRotatorLabel;
    QLabel* yTranslateLabel;
    QLabel* zoomLabel;

    // button for raytracing
    QPushButton* raytraceButton;

    void handleRaytrace() const;

public:
    RenderWindow(
        std::vector<ThreeDModel>* newTexturedObject,
        RenderParameters* newRenderParameters,
        const char* windowName = "Object Renderer");

    // routine to reset the interface
    // sets every visual control to match the model
    // gets called by the controller after each change in the model
    void resetInterface() const;

    // declare the render controller class a friend so it can access the UI elements
    friend class RenderController;
}; // class RenderWindow

#endif
