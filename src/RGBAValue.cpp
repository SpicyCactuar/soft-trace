#include "RGBAValue.h"

RGBAValue::RGBAValue()
    : red(0),
      green(0),
      blue(0),
      alpha(0) {
}

RGBAValue::RGBAValue(const unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
    : red(red),
      green(green),
      blue(blue),
      alpha(alpha) {
}


RGBAValue::RGBAValue(float red, float green, float blue, float alpha) {
    // values outside 0.0-255.0 are clamped
    if (red > 255.0f) {
        red = 255.0f;
    }
    if (red < 0.0f) {
        red = 0.0f;
    }
    this->red = static_cast<unsigned char>(red);

    if (green > 255.0f) {
        green = 255.0f;
    }
    if (green < 0.0f) {
        green = 0.0f;
    }
    this->green = static_cast<unsigned char>(green);

    if (blue > 255.0f) {
        blue = 255.0f;
    }
    if (blue < 0.0f) {
        blue = 0.0f;
    }
    this->blue = static_cast<unsigned char>(blue);

    if (alpha > 255.0f) {
        alpha = 255.0f;
    }
    if (alpha < 0.0f) {
        alpha = 0.0f;
    }
    this->alpha = static_cast<unsigned char>(alpha);
}

RGBAValue::RGBAValue(const RGBAValue& other)
    : red(other.red)
      , green(other.green)
      , blue(other.blue)
      , alpha(other.alpha) {
}

RGBAValue operator*(float scalar, const RGBAValue& colour) {
    // feed the new values into a constructor & return
    return RGBAValue(scalar * colour.red,
                     scalar * colour.green,
                     scalar * colour.blue,
                     scalar * colour.alpha);
}

RGBAValue operator+(const RGBAValue& left, const RGBAValue& right) {
    // compute new values, then use constructor to scale & clamp
    return RGBAValue(static_cast<float>(left.red) + static_cast<float>(right.red),
                     static_cast<float>(left.green) + static_cast<float>(right.green),
                     static_cast<float>(left.blue) + static_cast<float>(right.blue),
                     static_cast<float>(left.alpha) + static_cast<float>(right.alpha));
}

RGBAValue RGBAValue::modulate(const RGBAValue& right) const {
    float leftRed = red / 255.0f, leftGreen = green / 255.0f;
    float leftBlue = blue / 255.0f, leftAlpha = alpha / 255.0f;
    float rightRed = right.red / 255.0f, rightGreen = right.green / 255.0f;
    float rightBlue = right.blue / 255.0f, rightAlpha = right.alpha / 255.0f;

    // modulate them & convert back to 0..255
    return RGBAValue(255.0f * leftRed * rightRed,
                     255.0f * leftGreen * rightGreen,
                     255.0f * leftBlue * rightBlue,
                     255.0f * leftAlpha * rightAlpha);
}

std::istream& operator>>(std::istream& inStream, RGBAValue& value) {
    // we want to read integers, not characters, so we create a temporary variable
    int component;
    inStream >> component;
    value.red = static_cast<unsigned char>(component);
    inStream >> component;
    value.green = static_cast<unsigned char>(component);
    inStream >> component;
    value.blue = static_cast<unsigned char>(component);
    return inStream;
}


std::ostream& operator<<(std::ostream& outStream, const RGBAValue& value) {
    // we want to output as integers, not characters, so we create a temporary variable
    int component = 0;
    component = value.red;
    outStream << component << " ";
    component = value.green;
    outStream << component << " ";
    component = value.blue;
    outStream << component;
    return outStream;
}
