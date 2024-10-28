#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "RenderController.h"
#include "RenderParameters.h"
#include "RenderWindow.h"
#include "ThreeDModel.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " geometry texture|material" << std::endl;
        return 0;
    }

    // initialize QT
    QApplication renderApp(argc, argv);

    // read input files for the geometry & texture
    std::ifstream geometryFile(argv[1]);
    std::ifstream textureFile(argv[2]);

    if (!geometryFile.good() || !textureFile.good()) {
        std::cout << "Read failed for object " << argv[1] << " or texture " << argv[2] << std::endl;
        return 0;
    }

    // Read objects
    std::vector<ThreeDModel> texturedObjects;
    // if is actually passing a material. This will trigger the modified obj read code.
    if (std::string s = argv[2]; s.find(".mtl") != std::string::npos) {
        texturedObjects = ThreeDModel::readObjectStreamMaterial(geometryFile, textureFile);
    } else {
        std::cout << "Second file is not a material file!" << std::endl;
        return 0;
    }

    if (texturedObjects.empty()) {
        std::cout << "Read failed for object " << argv[1] << " or texture " << argv[2] << std::endl;
        return 0;
    }

    // Execute application
    RenderParameters renderParameters;

    renderParameters.findLights(texturedObjects);

    RenderWindow renderWindow(&texturedObjects, &renderParameters, argv[1]);
    RenderController renderController(&renderParameters, &renderWindow);

    renderWindow.showMaximized();
    renderWindow.show();

    return QApplication::exec();
}
