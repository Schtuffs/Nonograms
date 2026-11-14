#include "Utils.h"

#include <iostream>

namespace Utils {

Vector3 GridPos(float gridPercent)
{
    Vector3 pos;

    int min = std::min(GetRenderWidth(), GetRenderHeight());
    pos.z   = min * gridPercent;
    pos.x   = (GetRenderWidth() - pos.z) / 2.;
    pos.y   = (GetRenderHeight() - pos.z) / 2.;

    return pos;
}

Vector2 CenterText(const char* text, Font font, float fontSize, float fontSpacing, Vector2 center)
{
    Vector2 textSize = MeasureTextEx(font, text, fontSize, fontSpacing);
    center.x         = center.x - (textSize.x / 2.);
    center.y         = center.y - (textSize.y / 2.);

    return center;
}

} // namespace Utils
