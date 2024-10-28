QT+=opengl
TEMPLATE = app
TARGET = ./bin/soft-trace
# Add GLM to the INCLUDEPATH
INCLUDEPATH += /usr/include/glm
INCLUDEPATH += ./src
OBJECTS_DIR=./build/obj
MOC_DIR=./build/moc

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#adding openMP
QMAKE_CXXFLAGS+= -fopenmp -Wall
CONFIG += c++17
LIBS += -fopenmp

# Input
HEADERS += src/ArcBall.h \
           src/ArcBallWidget.h \
           src/Light.h \
           src/Material.h \
           src/Math.h \
           src/Random.h \
           src/Ray.h \
           src/RaytraceRenderWidget.h \
           src/RenderController.h \
           src/RenderParameters.h \
           src/RenderWidget.h \
           src/RenderWindow.h \
           src/RGBAImage.h \
           src/RGBAValue.h \
           src/Scene.h \
           src/SurfaceElement.h \
           src/ThreeDModel.h \
           src/Triangle.h

SOURCES += src/ArcBall.cpp \
           src/ArcBallWidget.cpp \
           src/Light.cpp \
           src/Material.cpp \
           src/Math.cpp \
           src/Random.cpp \
           src/Ray.cpp \
           src/RenderParameters.cpp \
           src/Scene.cpp \
           src/SurfaceElement.cpp \
           src/ThreeDModel.cpp \
           src/Triangle.cpp \
           src/main.cpp \
           src/RaytraceRenderWidget.cpp \
           src/RenderController.cpp \
           src/RenderWidget.cpp \
           src/RenderWindow.cpp \
           src/RGBAImage.cpp \
           src/RGBAValue.cpp
