// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "../HitInfo.h"
#include "SDF.h"

using namespace optix;


bool SDF::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{


	float threshold = 0.1;
	float maxDist = 50;
	
	float3 dir = normalize(r.direction);
	float3 pos = r.origin;

	float3 center2 = make_float3(0, 0.5, 0);

	float dist = min(distSphere(pos, center, radius), distSphere(pos, center2, radius));
	float distTraveled = 0;
	

	while (distTraveled < maxDist && dist > threshold) {
		pos = pos + dir * dist;
		distTraveled += dist;
		dist = min(distSphere(pos, center, radius), distSphere(pos, center2, radius));
	}

	if (dist > threshold) { 
		return false; 
	}
	
	hit.has_hit = true;
	hit.position = pos;
	float3 normal = normalize(pos - center);
	hit.dist = distTraveled;
	hit.geometric_normal = normal;
	hit.shading_normal = normal;
	hit.material = &material;
	//InvSphereMap map = InvSphereMap();
	//float u, v;
	//map.project_direction(normal, u, v);
	if (material.has_texture) {
		hit.texcoord = make_float3(hit.shading_normal.x, hit.shading_normal.y, hit.shading_normal.z);
	}
	return true;
}

void SDF::transform(const Matrix4x4& m)
{
	float3 radius_vec = make_float3(radius, 0.0f, 0.0f) + center;
	radius_vec = make_float3(m * make_float4(radius_vec, 1.0f));
	center = make_float3(m * make_float4(center, 1.0f));
	// The radius is scaled by the X scaling factor.
	// Not ideal, but the best we can do without elipsoids
	radius_vec -= center;
	radius = length(radius_vec);
}

Aabb SDF::compute_bbox() const
{
	Aabb bbox;
	bbox.include(center - make_float3(radius, 0.0f, 0.0f));
	bbox.include(center + make_float3(radius, 0.0f, 0.0f));
	bbox.include(center - make_float3(0.0f, radius, 0.0f));
	bbox.include(center + make_float3(0.0f, radius, 0.0f));
	bbox.include(center - make_float3(0.0f, 0.0f, radius));
	bbox.include(center + make_float3(0.0f, 0.0f, radius));
	return bbox;
}

float SDF::distSphere(const float3& pos, const float3 spherePos, const float radius) const{
	return abs(length(pos - spherePos) - radius);
}
