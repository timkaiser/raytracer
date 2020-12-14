#pragma once

#ifndef BLENDING_H
#define BLENDING_H

#include <optix_world.h>

enum Blendfunction { MIN, MAX, SMOOTHMIN};

float blend(Blendfunction func, float a, float b, float blendparameter = 0.0f) {
	switch (func) {
	case MIN:
		return minb(a, b);
		break;
	case MAX:
		return maxb(a, b);
		break;
	case SMOOTHMIN:
		return smoothMinb(a, b, blendparameter);
		break;
	}
}

float minb(float a, float b) {
	return fminf(a,b);
}

float maxb(float a, float b) {
	return fmaxf(a, b);
}

float smoothMinb(float a, float b, float k) {
	float h = fmaxf(k - abs(a - b), 0) / k;
	return fminf(a, b) - h * h * h * k / 6.0f;
}



#endif // BLENDING_H