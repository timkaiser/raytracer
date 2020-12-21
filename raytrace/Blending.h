#pragma once

#ifndef BLENDING_H
#define BLENDING_H

#include <optix_world.h>

//implements blend function
enum Blendfunction {UNION, INTERSECTION, SUBTRACTION};

float smoothUnion(float a, float b, float k);

float smoothIntersection(float a, float b, float k);

float smoothSubtraction(float a, float b);

float blend(Blendfunction func, float a, float b, float blendparameter = 0.0f);


#endif // BLENDING_H