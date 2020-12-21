#include "SDFObject.h"

using namespace optix;

//this file really shouldn't be necessary, but without it I had problems
void SDFObject::transform(const Matrix4x4& m) {
	std::cout << "SDFObject::transform: This methode should never have been called." << std::endl;
}

float SDFObject::distance(const float3& pos) const{
	std::cout << "SDFObject::distance: This methode should never have been called." << std::endl;
	return 1000;
}


void SDFObject::add_to_bbox(Aabb& bb) const {
	std::cout << "SDFObject::compute_bbox: This methode should never have been called." << std::endl;
}