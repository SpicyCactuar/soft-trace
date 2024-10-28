#include "ThreeDModel.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <glm/geometric.hpp>

#include "Math.h"

#define MAXIMUM_LINE_LENGTH 1024

ThreeDModel::ThreeDModel() {
    // TexturedObject()
    vertices.resize(0);
    normals.resize(0);
    textureCoords.resize(0);
}

std::vector<ThreeDModel> ThreeDModel::readObjectStreamMaterial(std::istream& geometryStream,
                                                               std::istream& materialStream) {
    std::vector<ThreeDModel> result;
    // First we read the material file
    std::vector<Material*> ms = Material::readMaterials(materialStream);
    Material* material = nullptr;
    ThreeDModel model;

    // Vertex data is shared between everyone
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> textureCoords;

    while (true) {
        char readBuffer[MAXIMUM_LINE_LENGTH];
        char firstChar = static_cast<char>(geometryStream.get());

        if (geometryStream.eof()) {
            break;
        }

        switch (firstChar) {
            // Comment
            case '#':
                // read and discard the line
                geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);
                break;

            // Vertex
            case 'v': {
                char secondChar = static_cast<char>(geometryStream.get());

                if (geometryStream.eof()) {
                    break;
                }

                switch (secondChar) {
                    // space - indicates a vertex
                    case ' ': {
                        glm::vec3 vertex;
                        geometryStream >> vertex;
                        vertices.push_back(vertex);
                        break;
                    }
                    // n indicates normal vector
                    case 'n': {
                        glm::vec3 normal;
                        geometryStream >> normal;
                        normals.push_back(glm::normalize(normal));
                        break;
                    }
                    // t indicates texture coords
                    case 't': {
                        // tex coord
                        glm::vec3 texCoord;
                        float u;
                        float v;
                        geometryStream >> u;
                        geometryStream >> v;
                        texCoord.x = u;
                        texCoord.y = v;
                        textureCoords.push_back(texCoord);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }

            // face data
            case 'f': {
                // a face can have an arbitrary number of vertices
                // so we will create a separate buffer to read from
                geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);

                // turn into a C++ string
                std::string lineString = std::string(readBuffer);

                // create a string stream
                std::stringstream lineParse(lineString);

                // create vectors for the IDs (with different names from the master arrays)
                std::vector<unsigned int> faceVertexSet;
                std::vector<unsigned int> faceNormalSet;
                std::vector<unsigned int> faceTexCoordSet;

                // loop through the line
                while (!lineParse.eof()) {
                    // the triple of vertex, normal, tex coord IDs
                    unsigned int vertexID;
                    unsigned int normalID;
                    unsigned int texCoordID;


                    lineParse >> vertexID;
                    lineParse.get();
                    if (lineParse.eof()) {
                        break;
                    }

                    lineParse >> texCoordID;
                    lineParse.get();
                    if (lineParse.eof()) {
                        break;
                    }

                    lineParse >> normalID;

                    // if we got this far, we presumably have three valid numbers, so add them
                    // but notice that .obj uses 1-based numbering, where our arrays use 0-based
                    faceVertexSet.push_back(vertexID - 1);
                    faceNormalSet.push_back(normalID - 1);
                    faceTexCoordSet.push_back(texCoordID - 1);
                }

                // as long as the face has at least three vertices, add to the master list
                if (faceVertexSet.size() > 2) {
                    model.faceVertices.push_back(faceVertexSet);
                    model.faceNormals.push_back(faceNormalSet);
                    model.faceTexCoords.push_back(faceTexCoordSet);
                }

                break;
            }

            // usemtl
            case 'u': {
                std::string token;
                // we discard until we find space
                geometryStream >> token;
                // then get the name of the material;
                geometryStream >> token;

                for (const auto& i : ms) {
                    if (i->name == token) {
                        if (material == nullptr) {
                            material = i;
                            model.material = material;
                            break;
                        } else {
                            model.vertices.assign(vertices.begin(), vertices.end());
                            model.normals.assign(normals.begin(), normals.end());
                            model.textureCoords.assign(textureCoords.begin(), textureCoords.end());
                            result.push_back(model);
                            model = ThreeDModel();
                            material = i;
                            model.material = material;
                        }
                    }
                }
            }
            default:
                break;
        }
    }

    model.material = material;
    model.vertices.assign(vertices.begin(), vertices.end());
    model.normals.assign(normals.begin(), normals.end());
    model.textureCoords.assign(textureCoords.begin(), textureCoords.end());
    result.push_back(model);
    return result;
}

// read routine object true on success, NULL otherwise
std::vector<ThreeDModel> ThreeDModel::readObjectStream(std::istream& geometryStream) {
    ThreeDModel model;
    model.material = nullptr;

    while (true) {
        char readBuffer[MAXIMUM_LINE_LENGTH];

        char firstChar = static_cast<char>(geometryStream.get());

        // check for eof() in case we've run out
        if (geometryStream.eof()) {
            break;
        }

        switch (firstChar) {
            // Comment
            case '#':
                // read and discard the line
                geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);
                break;

            // Vertex
            case 'v': {
                char secondChar = static_cast<char>(geometryStream.get());

                if (geometryStream.eof()) {
                    break;
                }

                switch (secondChar) {
                    // space - indicates a vertex
                    case ' ': {
                        glm::vec3 vertex;
                        geometryStream >> vertex;
                        model.vertices.push_back(vertex);
                        break;
                    }

                    // n indicates normal vector
                    case 'n': {
                        glm::vec3 normal;
                        geometryStream >> normal;
                        model.normals.push_back(normal);
                        break;
                    }

                    // t indicates texture coords
                    case 't': {
                        glm::vec3 texCoord;
                        geometryStream >> texCoord;
                        model.textureCoords.push_back(texCoord);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }

            // face data
            case 'f': {
                // a face can have an arbitrary number of vertices
                // so we will create a separate buffer to read from
                geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);

                // turn into a C++ string
                std::string lineString = std::string(readBuffer);

                // create a string stream
                std::stringstream lineParse(lineString);

                // create vectors for the IDs (with different names from the master arrays)
                std::vector<unsigned int> faceVertexSet;
                std::vector<unsigned int> faceNormalSet;
                std::vector<unsigned int> faceTexCoordSet;

                // now loop through the line
                while (!lineParse.eof()) {
                    // the triple of vertex, normal, tex coord IDs
                    unsigned int vertexID;
                    unsigned int normalID;
                    unsigned int texCoordID;

                    // try reading them in, breaking if we hit eof
                    lineParse >> vertexID;
                    // retrieve & discard a slash
                    lineParse.get();

                    if (lineParse.eof()) {
                        break;
                    }

                    lineParse >> texCoordID;
                    lineParse.get();
                    if (lineParse.eof()) {
                        break;
                    }

                    lineParse >> normalID;

                    // if we got this far, we presumably have three valid numbers, so add them
                    // but notice that .obj uses 1-based numbering, where our arrays use 0-based
                    faceVertexSet.push_back(vertexID - 1);
                    faceNormalSet.push_back(normalID - 1);
                    faceTexCoordSet.push_back(texCoordID - 1);
                }

                // as long as the face has at least three vertices, add to the master list
                if (faceVertexSet.size() > 2) {
                    model.faceVertices.push_back(faceVertexSet);
                    model.faceNormals.push_back(faceNormalSet);
                    model.faceTexCoords.push_back(faceTexCoordSet);
                }

                break;
            }
            default:
                break;
        }
    }

    return {model};
}

void ThreeDModel::writeObjectStream(std::ostream& geometryStream) const {
    for (unsigned int vertex = 0; vertex < vertices.size(); vertex++) {
        geometryStream << "v  " << std::fixed << vertices[vertex] << std::endl;
    }
    geometryStream << "# " << vertices.size() << " vertices" << std::endl;
    geometryStream << std::endl;

    for (unsigned int normal = 0; normal < normals.size(); normal++) {
        geometryStream << "vn " << std::fixed << normals[normal] << std::endl;
    }
    geometryStream << "# " << normals.size() << " vertex normals" << std::endl;
    geometryStream << std::endl;

    for (unsigned int texCoord = 0; texCoord < textureCoords.size(); texCoord++) {
        geometryStream << "vt " << std::fixed << textureCoords[texCoord] << std::endl;
    }
    geometryStream << "# " << textureCoords.size() << " texture coords" << std::endl;
    geometryStream << std::endl;

    for (unsigned int face = 0; face < faceVertices.size(); face++) {
        geometryStream << "f ";

        // loop through # of vertices
        for (unsigned int vertex = 0; vertex < faceVertices[face].size(); vertex++) {
            geometryStream << faceVertices[face][vertex] + 1 << "/" << faceTexCoords[face][vertex] + 1 << "/"
                    << faceNormals[face][vertex] + 1 << " ";
        }

        geometryStream << std::endl;
    }
    geometryStream << "# " << faceVertices.size() << " polygons" << std::endl;
    geometryStream << std::endl;
}

void ThreeDModel::render() const {
    float emissiveColour[4];
    float specularColour[4];
    float diffuseColour[4];
    float ambientColour[4];

    // default ambient | diffuse | specular colour | opaque alpha
    float surfaceColour[4] = {0.7f, 0.7f, 0.7f, 1.0f};

    float specularExp = 0.0;
    if (material == nullptr) {
        // copy the intensity into RGB channels
        emissiveColour[0] = emissiveColour[1] = emissiveColour[2] = 0.0f;
        emissiveColour[3] = 1.0; // alpha

        ambientColour[0] = ambientColour[1] = ambientColour[2] = 0.5f;
        ambientColour[3] = 1.0; // alpha

        diffuseColour[0] = diffuseColour[1] = diffuseColour[2] = 0.5f;
        diffuseColour[3] = 1.0; // alpha

        specularColour[0] = specularColour[1] = specularColour[2] = 0.5f;
        specularColour[3] = 1.0; // alpha

        specularExp = 1.0f;
    } else {
        emissiveColour[0] = material->emissive.x;
        emissiveColour[1] = material->emissive.y;
        emissiveColour[2] = material->emissive.z;
        emissiveColour[3] = 1.0; // alpha

        ambientColour[0] = material->ambient.x;
        ambientColour[1] = material->ambient.y;
        ambientColour[2] = material->ambient.z;
        ambientColour[3] = 1.0; // alpha

        diffuseColour[0] = material->diffuse.x;
        diffuseColour[1] = material->diffuse.y;
        diffuseColour[2] = material->diffuse.z;
        diffuseColour[3] = 1.0; // alpha

        specularColour[0] = material->specular.x;
        specularColour[1] = material->specular.y;
        specularColour[2] = material->specular.z;
        specularColour[3] = 1.0; // alpha

        specularExp = material->shininess;
    }

    // we assume a single material for the entire object
    glMaterialfv(GL_FRONT, GL_EMISSION, emissiveColour);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColour);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColour);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularColour);
    glMaterialf(GL_FRONT, GL_SHININESS, specularExp);

    // repeat this for colour - extra call, but saves if statements
    glColor3fv(surfaceColour);

    for (unsigned int face = 0; face < faceVertices.size(); face++) {
        glBegin(GL_TRIANGLE_FAN);
        for (unsigned int faceVertex = 0; faceVertex < faceVertices[face].size(); faceVertex++) {
            glNormal3f(
                normals[faceNormals[face][faceVertex]].x,
                normals[faceNormals[face][faceVertex]].y,
                normals[faceNormals[face][faceVertex]].z);

            // set the texture coordinate
            glTexCoord2f(
                textureCoords[faceTexCoords[face][faceVertex]].x,
                textureCoords[faceTexCoords[face][faceVertex]].y);

            // and set the vertex position
            glVertex3f(
                vertices[faceVertices[face][faceVertex]].x,
                vertices[faceVertices[face][faceVertex]].y,
                vertices[faceVertices[face][faceVertex]].z);
        }
        glEnd();
    }
}
