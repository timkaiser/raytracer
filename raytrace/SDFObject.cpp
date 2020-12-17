#include "SDFObject.h"

using namespace optix;


void SDFObject::transform(const Matrix4x4& m) {
	std::cout << "SDFObject::transform: This methode should never have been called." << std::endl;
}

float SDFObject::distance(const float3& pos) const{
	std::cout << "SDFObject::distance: This methode should never have been called." << std::endl;
	return 1000;
}