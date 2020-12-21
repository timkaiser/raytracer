#include <optix_world.h>
#include <iostream>
#include "SDFTorus.h"

using namespace optix;

void SDFTorus::transform(const Matrix4x4& m) {
    center = make_float3(m * make_float4(center, 1.0f));
}


float SDFTorus::distance(const float3& pos) const {
    float3 q = make_float3(
        fmaxf(0, abs(pos.x - center.x) - bounds.x), 
        fmaxf(0, abs(pos.y - center.y) - bounds.y), 
        fmaxf(0, abs(pos.z - center.z) - bounds.z));

    return length(q) + min(max(q.x, max(q.y, q.z)), 0.0);
}

void SDFTorus::add_to_bbox(Aabb& bbox) const {
    bbox.include(center + make_float3(bounds.x, bounds.y, bounds.z));
    bbox.include(center + make_float3(bounds.x, bounds.y, -bounds.z));
    bbox.include(center + make_float3(bounds.x, -bounds.y, bounds.z));
    bbox.include(center + make_float3(bounds.x, -bounds.y, -bounds.z));
    bbox.include(center + make_float3(-bounds.x, bounds.y, bounds.z));
    bbox.include(center + make_float3(-bounds.x, bounds.y, -bounds.z));
    bbox.include(center + make_float3(-bounds.x, -bounds.y, bounds.z));
    bbox.include(center + make_float3(-bounds.x, -bounds.y, -bounds.z));
}