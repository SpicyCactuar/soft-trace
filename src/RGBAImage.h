#ifndef RGBAIMAGE_H
#define RGBAIMAGE_H

#include "RGBAValue.h"

/*
 * A minimal class for an image in single-byte RGBA format
 * Optimized for simplicity, not speed or memory
 * With read/write for ASCII RGBA files
 */
class RGBAImage {
public:
    RGBAValue* block;

    long width, height;

    RGBAImage();

    RGBAImage(const RGBAImage& other);

    ~RGBAImage();

    bool resize(long width, long height);

    RGBAValue* operator[](int rowIndex);

    const RGBAValue* operator[](int rowIndex) const;

    // routine to retrieve an interpolated texel value
    // assumes that u,v coordinates are in range of [0..1]
    // if the flag is not set, it will use nearest neighbour
    RGBAValue getTexel(float u, float v, bool bilinearFiltering);

    bool readPPM(std::istream& inStream);

    void writePPM(std::ostream& outStream);

    void clear(const RGBAValue& color);
};

#endif
