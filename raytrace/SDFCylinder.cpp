#include <optix_world.h>
#include <iostream>
#include "SDFCylinder.h"

using namespace optix;

void SDFCylinder::transform(const Matrix4x4& m) {
    center = make_float3(m * make_float4(center, 1.0f));
}

float SDFCylinder::distance(const float3& pos) const {
    float3  ba = axis - center;
    float3  pa = pos - center;
    float baba = dot(ba, ba);
    float paba = dot(pa, ba);
    float x = length(pa * baba - ba * paba) - radius * baba;
    float y = abs(paba - baba * 0.5) - baba * 0.5;
    float x2 = x * x;
    float y2 = y * y * baba;
    float d = (max(x, y) < 0.0) ? -min(x2, y2) : (((x > 0.0) ? x2 : 0.0) + ((y > 0.0) ? y2 : 0.0));
    return d<0? -1 : d==0 ? 0 : 1 * sqrt(abs(d)) / baba;

}

void SDFCylinder::add_to_bbox(Aabb& bbox) const {
    bbox.include(center + axis + make_float3(radius, radius, radius));
    bbox.include(center + axis + make_float3(radius, radius, -radius));
    bbox.include(center + axis + make_float3(radius, -radius, radius));
    bbox.include(center + axis + make_float3(radius, -radius, -radius));
    bbox.include(center + axis + make_float3(-radius, radius, radius));
    bbox.include(center + axis + make_float3(-radius, radius, -radius));
    bbox.include(center + axis + make_float3(-radius, -radius, radius));
    bbox.include(center + axis + make_float3(-radius, -radius, -radius));


    bbox.include(center - axis + make_float3(radius, radius, radius));
    bbox.include(center - axis + make_float3(radius, radius, -radius));
    bbox.include(center - axis + make_float3(radius, -radius, radius));
    bbox.include(center - axis + make_float3(radius, -radius, -radius));
    bbox.include(center - axis + make_float3(-radius, radius, radius));
    bbox.include(center - axis + make_float3(-radius, radius, -radius));
    bbox.include(center - axis + make_float3(-radius, -radius, radius));
    bbox.include(center - axis + make_float3(-radius, -radius, -radius));

}