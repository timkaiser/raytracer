// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "ObjMaterial.h"
#include "fresnel.h"
#include "RayTracer.h"

using namespace optix;

bool RayTracer::trace_reflected(const Ray& in, const HitInfo& in_hit, Ray& out, HitInfo& out_hit) const
{
    // Initialize the reflected ray and trace it.
    //
    // Input:  in         (the ray to be reflected)
    //         in_hit     (info about the ray-surface intersection)
    //
    // Output: out        (the reflected ray)
    //         out_hit    (info about the reflected ray)
    //
    // Return: true if the reflected ray hit anything
    //
    // Hints: (a) There is a reflect function available in the OptiX math library.
    //        (b) Set out_hit.ray_ior and out_hit.trace_depth.
    out = Ray(in_hit.position, optix::reflect(in.direction, in_hit.shading_normal), 0, 0.0001, RT_DEFAULT_MAX);  
    out_hit.ray_ior = in_hit.ray_ior;
    out_hit.trace_depth = in_hit.trace_depth + 1;
    return this->trace_to_closest(out, out_hit);
}

bool RayTracer::trace_refracted(const Ray& in, const HitInfo& in_hit, Ray& out, HitInfo& out_hit) const
{
    // Initialize the refracted ray and trace it.
    //
    // Input:  in         (the ray to be refracted)
    //         in_hit     (info about the ray-surface intersection)
    //
    // Output: out        (the refracted ray)
    //         out_hit    (info about the refracted ray)
    //
    // Return: true if the refracted ray hit anything
    //
    // Hints: (a) There is a refract function available in the OptiX math library.
    //        (b) Set out_hit.ray_ior and out_hit.trace_depth.
    //        (c) Remember that the function must handle total internal reflection.
   /*out = Ray(in_hit.position, make_float3(0) , 0, 0.0001, in.tmax);
    out_hit.ray_ior = in_hit.ray_ior;
    out_hit.trace_depth = in_hit.trace_depth + 1;
    float3 normal = in_hit.shading_normal;

    return optix::refract(out.direction, in.direction, in_hit.shading_normal, get_ior_out(in, in_hit, normal)/in_hit.ray_ior) && this->trace_to_closest(out, out_hit);;
	*/
	float3 normal = in_hit.shading_normal;
	float ior = get_ior_out(in, in_hit, normal);
	float3 refDir;
	if (optix::refract(refDir, in.direction, normal, ior / in_hit.ray_ior)) {
		out = Ray(in_hit.position, refDir, 0, 0.0001, RT_DEFAULT_MAX);
		out_hit.ray_ior = ior;
		out_hit.trace_depth = in_hit.trace_depth + 1;
		return trace_to_closest(out, out_hit);
    }else{
		out = Ray(in_hit.position, refDir, 0, 0.0001, RT_DEFAULT_MAX);
        return false;
    }
	
}

bool RayTracer::trace_refracted(const Ray& in, const HitInfo& in_hit, Ray& out, HitInfo& out_hit, float& R) const
{
  // Initialize the refracted ray and trace it.
  // Compute the Fresnel reflectance (see fresnel.h) and return it in R.
  //
  // Input:  in         (the ray to be refracted)
  //         in_hit     (info about the ray-surface intersection)
  //
  // Output: out        (the refracted ray)
  //         out_hit    (info about the refracted ray)
  //
  // Return: true if the refracted ray hit anything
  //
  // Hints: (a) There is a refract function available in the OptiX math library.
  //        (b) Set out_hit.ray_ior and out_hit.trace_depth.
  //        (c) Remember that the function must handle total internal reflection.
	if(trace_refracted(in, in_hit, out, out_hit) || !(out.direction.x == 0 && out.direction.y == 0 && out.direction.z == 0)) { //will this lead to problems?
        float cosOi = dot(normalize(in_hit.shading_normal), normalize(in.direction));
        float cosOt = dot(normalize(in_hit.shading_normal), normalize(out.direction));
        float ni = in_hit.ray_ior;
        float nt = out_hit.ray_ior;
        float rperp = (ni * cosOi - nt * cosOt) / (ni * cosOi + nt * cosOt);
        float rpara = (nt * cosOi - ni * cosOt) / (nt * cosOi + ni * cosOt);
        R = 0.5f * (pow(abs(rperp), 2) + pow(abs(rpara), 2));
    }else { //total internal reflection 
        R = 1.0f;
        return false;
    }
}

float RayTracer::get_ior_out(const Ray& in, const HitInfo& in_hit, float3& normal) const
{
  normal = in_hit.shading_normal;
	if(dot(normal, in.direction) > 0.0)
	{
    normal = -normal;
    return 1.0f;
  }
  const ObjMaterial* m = in_hit.material;
  return m ? m->ior : 1.0f;
}
