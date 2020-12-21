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

//calculates ray object intersections
bool SDF::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
	float threshold = 0.00001;		//How close we have to get to the surface until it counts as a hit
	float maxDist = 50;				//After what distance we give up looking for surfaces
	
	float3 dir = normalize(r.direction);	//normalizing the direction is important 
	float3 pos = r.origin + dir * r.tmin;	//making sure that we take the minimum distance into account. important for reflecting


	float dist = abs(distance(pos));		//abs(), because we could be inside the object
	float distTraveled = 0;					//to measure the distance to the object
	
	hit.closest_dist = dist;				//only needed for sdf visualization
	
	//Ray Marching
	while (distTraveled < maxDist && dist > threshold) {
		pos = pos + dir * dist;
		distTraveled += dist;
		dist = abs(distance(pos));
		hit.closest_dist = fminf(hit.closest_dist, dist);
	}

	//no hit
	if (dist > threshold) { 
		return false; 
	}
	
	pos = pos + dir * dist; //trying to minimize the error. It worked. Kind of.

	//register hit
	hit.closest_dist = abs(distance(pos));
	hit.has_hit = true;
	hit.position = pos;
	float3 normal = calcNormal(pos);
	hit.dist = distTraveled;
	hit.geometric_normal = normal;
	hit.shading_normal = normal;
	hit.material = &material;

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
	for each (SDFObject * obj in sdfObjects) {
		obj->add_to_bbox(bbox);
	}
	return bbox;
}

//This methode returns the derivative of the distance function, and therefor the normal, at point pos
float3 SDF::calcNormal(const float3& pos) const {
	const float h = 1e-4; //epsilon
	float dist = distance(pos);     //f(x,y,z) = distance(float3 position)
	float3 normal = make_float3(
		distance(make_float3(pos.x + h, pos.y, pos.z)) - dist,
		distance(make_float3(pos.x, pos.y + h, pos.z)) - dist,
		distance(make_float3(pos.x, pos.y, pos.z + h)) - dist
		);

	//not sure if necessary, but want to be sure that the normal is in the right direction
	if (distance(pos + normal) < dist) {
		return  -normalize(normal);
	}
	else {
		return  normalize(normal);
	}
}

//This methode retuns the signed distance to any surface in the scene at position pos 
float SDF::distance(const float3& pos) const {
	float dist = 100000;
	for each (SDFObject* obj in sdfObjects)	{
		float distObj = obj->distance(pos);
		dist = abs(distObj) < abs(dist) ? distObj : dist;
	}
	return dist;
}


