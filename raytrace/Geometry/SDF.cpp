// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "../HitInfo.h"
#include "SDF.h"
#include <iostream>
#include "../SDFObject.h"
#include "../SDFCSGTree.h"
#include "../SDFSphere.h"
#include "../SDFBox.h"

using namespace optix;

bool debug = true;

bool SDF::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
	float threshold = 0.0001;
	float maxDist = 50;
	
	float3 dir = normalize(r.direction);
	float3 pos = r.origin;


	float dist = distance(pos);
	float distTraveled = 0;
	
	
	while (distTraveled < maxDist && dist > threshold) {
		pos = pos + dir * dist;
		distTraveled += dist;
		dist = distance(pos);
	}

	if (dist > threshold) { 
		return false; 
	}
	
	hit.has_hit = true;
	hit.position = pos;
	float3 normal = calcNormal(pos);
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
	std::cout << "transform SDF" << std::endl;
	root->transform(m);
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

float3 SDF::calcNormal(const float3& pos) const {
	const float h = 1e-4;
	float dist = distance(pos);
	float3 normal = make_float3(
			distance(make_float3(pos.x + h, pos.y, pos.z)) - dist,
			distance(make_float3(pos.x, pos.y + h, pos.z)) - dist,
			distance(make_float3(pos.x, pos.y, pos.z + h)) - dist
		);
	return normalize(normal);
}


float SDF::distance(const float3& pos) const {
	return abs(root->distance(pos));
}


void SDF::buildSDF(const int w, const int l, const int h, float r){
	//root = new SDFSphere(make_float3(-0.5f, 0, 0),0.5f);
	SDFObject* a1 = new SDFBox(make_float3(0, 0, 0), 0.5f, 0.5f, 0.5f);
	SDFObject* a2 = new SDFSphere(make_float3(0.0f, 0, 0.0f), 0.9f);
	SDFObject* a = new SDFCSGTree(a1, a2, Blendfunction::INTERSECTION, 0.1f);
	SDFObject* b = new SDFSphere(make_float3(0.0f, 0, 0.0f), 0.5f);


	root = new SDFCSGTree(a, b, Blendfunction::SUBTRACTION, 0.1f);

	//root = new SDFBox(make_float3(-0.5f, 0, 0), make_float3(1));

}


