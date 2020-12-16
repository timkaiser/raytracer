#include "SDFObject.h"

using namespace optix;

float SDFObject::distance(const float3& pos) const{
	std::cout << "SDFObject::distance: This methode should never have been called." << std::endl;
	return 1000;
}