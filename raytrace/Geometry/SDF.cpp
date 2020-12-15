// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "../HitInfo.h"
#include "SDF.h"
#include <iostream>
#include "../SDFObject.h"
//#include "../SDFCSGTree.h"
#include "../SDFSphere.h"

using namespace optix;

bool debug = true;

bool SDF::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{

	/*if (debug) {
		SDF
	}*/


	float threshold = 0.1;
	float maxDist = 50;
	
	float3 dir = normalize(r.direction);
	float3 pos = r.origin;

	/*float dist = outsideDistToField(pos);
	if (dist < 0) {
		pos = pos + dir * dist;
	}*/

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

float SDF::distSphere(const float3& pos, const float3 spherePos, const float radius) const {
	return length(pos - spherePos) - radius;
}

float smoothMin(float a, float b, float k) {
	float h = max(k - abs(a - b), 0) / k;
	return min(a, b) - h * h * h * k / 6.0f;
}

float SDF::distance(const float3& pos) const {
	return root.distance(pos);
}


int arr_width, arr_length, arr_height;
int arrayCoords(int x, int y, int z) {
	return x * arr_width + y * arr_length + z;
}

float arr_resolution;
float* field;

void SDF::buildSDF(const int w, const int l, const int h, float r){
	root = SDFSphere(make_float3(0, 0, 0), 1);


	/*arr_width = w;
	arr_length = l;
	arr_height = h;

	field = new float[w*l*h];
	for (int i = 0; i < arr_width; i++) {
		for (int j = 0; j < arr_length; j++) {
			for (int k = 0; k < arr_height; k++) {
				float3 pos = make_float3((i-w/2.0) * r, (j - l / 2.0) * r, (k - h / 2.0) * r);
				field[arrayCoords(i, j, k)] = distance(pos);
			}
		}
	}


	for (int i = 0; i < arr_width; i+=2) {
		for (int j = 0; j < arr_length; j += 2) {
			for (int k = 0; k < arr_height; k += 2) {
				std::cout << field[arrayCoords(i, j, k)] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n\n";
	}*/
}


float SDF::outsideDistToField(const float3& rayPos) const {
	float3 q = make_float3(
		abs(rayPos.x) - center.x - (arr_width - 1) * arr_resolution * 0.5,
		abs(rayPos.y) - center.y - (arr_length - 1) * arr_resolution * 0.5,
		abs(rayPos.z) - center.z - (arr_height - 1) * arr_resolution * 0.5
		);
	return length(fmaxf(q, make_float3(0.0))) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float SDF::retrieveDistance(const float3& rayPos) const {
	float distOutside = outsideDistToField(rayPos);
	if (distOutside < 0) {
		return distOutside;
	}

	float3 pos = rayPos - center +make_float3(arr_width - 1, arr_length - 1, arr_height - 1) * 0.5f;
	if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x > arr_width - 1 || pos.y > arr_length - 1 || pos.z >arr_height - 1) {
		float3 result;
		result.x = pos.x < 0 ? -pos.x : pos.x > arr_width - 1 ? (arr_width - 1) - pos.x : 0;
		result.y = pos.y < 0 ? -pos.y : pos.y > arr_length - 1 ? (arr_length - 1) - pos.y : 0;
		result.z = pos.z < 0 ? -pos.z : pos.z > arr_height - 1 ? (arr_height - 1) - pos.z : 0;

		return length(result);
	}

	int idx = (int)pos.x;
	int idy = (int)pos.y;
	int idz = (int)pos.z;

	float px = pos.x - idx;
	float py = pos.y - idy;
	float pz = pos.z - idz;

	float d1 = field[arrayCoords(idx, idy, idz)];
	float d2 = field[arrayCoords(idx, idy, idz + 1)];
	float d3 = field[arrayCoords(idx, idy + 1, idz)];
	float d4 = field[arrayCoords(idx, idy + 1, idz + 1)];
	float d5 = field[arrayCoords(idx + 1, idy, idz)];
	float d6 = field[arrayCoords(idx + 1, idy, idz + 1)];
	float d7 = field[arrayCoords(idx + 1, idy + 1, idz)];
	float d8 = field[arrayCoords(idx + 1, idy + 1, idz + 1)];

	return d1 * (1 - px) * (1 - py) * (1 - pz)
		+ d2 * (1 - px) * (1 - py) * (pz)
		+ d3 * (1 - px) * (py) * (1 - pz)
		+ d4 * (1 - px) * (py) * (pz)
		+ d5 * (px) * (1 - py) * (1 - pz)
		+ d6 * (px) * (1 - py) * (pz)
		+ d7 * (px) * (py) * (1 - pz)
		+ d8 * (px) * (py) * (pz);
}



