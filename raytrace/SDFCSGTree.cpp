#include <optix_world.h>
#include <iostream>
#include "SDFCSGTree.h"

using namespace optix;

void SDFCSGTree::transform(const Matrix4x4& m) {
	if (left != nullptr) {
		left->transform(m);
	}

	if (right != nullptr) {
		right->transform(m);
	}
}


float SDFCSGTree::distance(const float3& pos) const{
	return blend(blendfunction, left->distance(pos), right->distance(pos), blendparameter);
}