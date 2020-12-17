#include <optix_world.h>
#include <iostream>
#include "SDFSphere.h"

using namespace optix;

void SDFSphere::transform(const Matrix4x4& m){
    std::cout << "transform SDF Shpere" << std::endl;
    float3 radius_vec = make_float3(radius, 0.0f, 0.0f) + center;
    radius_vec = make_float3(m * make_float4(radius_vec, 1.0f));
    center = make_float3(m * make_float4(center, 1.0f));
    // The radius is scaled by the X scaling factor.
    // Not ideal, but the best we can do without elipsoids
    radius_vec -= center;
    radius = length(radius_vec);
}

float SDFSphere::distance(const float3& pos) const{
	return length(pos - center) - radius;
}