#include "Blending.h"

float smoothUnion(float a, float b, float k) {
	if (k == 0) {
		return fminf(a, b);
	}
	float h = fmaxf(k - abs(a - b), 0.0);
	return fminf(a, b) - h * h * 0.25 / k;
}

float smoothIntersection(float a, float b, float k) {
	if (k == 0) {
		return fmaxf(a, b);
	}
	float h = fmaxf(k - abs(a - b), 0.0);
	return fmaxf(a, b) + h * h * 0.25 / k;
}

float smoothSubtraction(float a, float b, float k) {
	if (k == 0) {
		return fmaxf(a, -b);
	}
	float h = fmaxf(k - abs(a + b), 0.0);
	return fmaxf(a, -b) + h * h * 0.25 / k;
}

float blend(Blendfunction func, float a, float b, float smoothing) {
	switch(func){
		case UNION:
			return smoothUnion(a, b, smoothing);
			break;
		case INTERSECTION:
			return smoothIntersection(a, b, smoothing);
			break;
		case SUBTRACTION:
			return smoothSubtraction(a, b, smoothing);
			break;
	}
}