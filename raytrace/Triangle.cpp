// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "Triangle.h"

using namespace optix;

bool intersect_triangle(const Ray& ray, 
                        const float3& v0, 
                        const float3& v1, 
                        const float3& v2, 
                        float3& n,
                        float& t,
                        float& v,
                        float& w)
{
  // Implement ray-triangle intersection here (see Listing 1 in the lecture note).
  // Note that OptiX also has an implementation, so you can get away
  // with not implementing this function. However, I recommend that
  // you implement it for completeness.

  return false;
}


bool Triangle::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
	// Implement ray-triangle intersection here.
	//
	// Input:  r                    (the ray to be checked for intersection)
	//         prim_idx             (index of the primitive element in a collection, not used here)
	//
	// Output: hit.has_hit          (set true if the ray intersects the triangle)
	//         hit.dist             (distance from the ray origin to the intersection point)
	//         hit.position         (coordinates of the intersection point)
	//         hit.geometric_normal (the normalized normal of the triangle)
	//         hit.shading_normal   (the normalized normal of the triangle)
	//         hit.material         (pointer to the material of the triangle)
	//        (hit.texcoord)        (texture coordinates of intersection point, not needed for Week 1)
	//
	// Return: True if the ray intersects the triangle, false otherwise
	//
	// Relevant data fields that are available (see Triangle.h)
	// r                            (the ray)
	// v0, v1, v2                   (triangle vertices)
	// (t0, t1, t2)                 (texture coordinates for each vertex, not needed for Week 1)
	// material                     (material of the triangle)
	//
	// Hint: Use the function intersect_triangle(...) to get the hit info.
	//       Note that you need to do scope resolution (optix:: or just :: in front
	//       of the function name) to choose between the OptiX implementation and
	//       the function just above this one.
	float dist, v, w;
	float3 normal = normalize(cross(v1 - v0, v0 - v2));
	optix::intersect_triangle(r, v0, v1, v2,normal ,dist, v, w);

	if (v < 0 || w < 0 || v + w>1) { return false; }

	//set hit parameter
	hit.has_hit = true;
	hit.dist = dist;
	hit.geometric_normal = normal;
	hit.shading_normal = normal;
	hit.position = r.origin + dist * r.direction;
	hit.material = &material;
	hit.texcoord = make_float3(0, 0, 0); //TODO: change


	return true;
}

void Triangle::transform(const Matrix4x4& m) 
{ 
  v0 = make_float3(m*make_float4(v0, 1.0f)); 
  v1 = make_float3(m*make_float4(v1, 1.0f)); 
  v2 = make_float3(m*make_float4(v2, 1.0f)); 
}

Aabb Triangle::compute_bbox() const
{
  Aabb bbox;
  bbox.include(v0);
  bbox.include(v1);
  bbox.include(v2);
  return bbox;
}
