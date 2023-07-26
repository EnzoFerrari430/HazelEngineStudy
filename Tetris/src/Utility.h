#pragma once

#include <glm/gtc/matrix_transform.hpp>

unsigned int Vec4toImU32(const glm::vec4& rgba)
{
    unsigned int result;

    float a = rgba.a;
    float b = rgba.b;
    float g = rgba.g;
    float r = rgba.r;

    result = (unsigned int)(255 * a);
    result <<= 8;

    result += (unsigned int)(255 * b);
    result <<= 8;

    result += (unsigned int)(255 * g);
    result <<= 8;

    result += (unsigned int)(255 * r);
    return result;
}