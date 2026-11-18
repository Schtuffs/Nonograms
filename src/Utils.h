#pragma once

#include "raylib.h"

namespace Utils {

template <typename T>
T Clamp(T value, T min, T max)
{
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

// Fraction of screen the grid takes up (based on min of width and height).
Vector3 GridPos(float gridPercent);

// Centers text automatically
Vector2 CenterText(const char* text, Font font, float fontSize, float fontSpacing, Vector2 center);
} // namespace Utils
