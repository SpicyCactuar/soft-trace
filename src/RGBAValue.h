#ifndef RGBAVALUE_H
#define RGBAVALUE_H

#include <iostream>

class RGBAValue {
public:
    unsigned char red, green, blue, alpha;

    RGBAValue();

    // value constructor with default alpha
    RGBAValue(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);

    // values outside 0.0-255.0 are clamped
    RGBAValue(float red, float green, float blue, float alpha);

    RGBAValue(const RGBAValue& other);

    // NB: this routine scales each component by 1/255.0, multiplies then inverts
    RGBAValue modulate(const RGBAValue& right) const;
};

RGBAValue operator*(float scalar, const RGBAValue& colour);

RGBAValue operator+(const RGBAValue& left, const RGBAValue& right);

// WARNING - always sets alpha to 255
std::istream& operator>>(std::istream& inStream, RGBAValue& value);

// WARNING - does not output alpha
std::ostream& operator<<(std::ostream& outStream, const RGBAValue& value);

#endif
