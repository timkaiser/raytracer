#include <optix_world.h>
#include <iostream>
#include "SDFCSGTree.h"
#include "Blending.h"

using namespace optix;

float SDFCSGTree::distance(float3& pos) const{
	return length(pos) - 1;
}