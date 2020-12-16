#include <optix_world.h>
#include <iostream>
#include "SDFSphere.h"

using namespace optix;

float SDFSphere::distance(const float3& pos) const {
	return length(pos - center) - radius;
}