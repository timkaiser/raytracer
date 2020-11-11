// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "int_pow.h"
#include "GlossyVolume.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 GlossyVolume::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  // Compute the specular part of the glossy shader and attenuate it
  // by the transmittance of the material if the ray is inside (as in
  // the volume shader).
	if (hit.trace_depth >= max_depth)
		return make_float3(0.0f);

	float R;
	Ray reflected, refracted;
	HitInfo hit_reflected, hit_refracted;
	tracer->trace_reflected(r, hit, reflected, hit_reflected);
	tracer->trace_refracted(r, hit, refracted, hit_refracted, R);
	float3 result = R * (shadePhong(r, hit, emit) + shade_new_ray(reflected, hit_reflected)) + (1.0f - R) * shade_new_ray(refracted, hit_refracted);
	
	if (dot(hit.shading_normal, r.direction) > 0) {
		result *= get_transmittance(hit);
	}
	
	return result;
}

float3 GlossyVolume::shadePhong(const Ray& r, HitInfo& hit, bool emit) const
{
	float3 rho_s = get_specular(hit);
	float s = get_shininess(hit);

	// Implement Phong reflection here.
	//
	// Input:  r          (the ray that hit the material)
	//         hit        (info about the ray-surface intersection)
	//         emit       (passed on to Emission::shade)
	//
	// Return: radiance reflected to where the ray was coming from
	//
	// Relevant data fields that are available (see Lambertian.h, Ray.h, and above):
	// lights             (vector of pointers to the lights in the scene)
	// hit.position       (position where the ray hit the material)
	// hit.shading_normal (surface normal where the ray hit the material)
	// rho_d              (difuse reflectance of the material)
	// rho_s              (specular reflectance of the material)
	// s                  (shininess or Phong exponent of the material)
	//
	// Hint: Call the sample function associated with each light in the scene.
	float3 result = make_float3(0);
	for (Light* light : lights) {
		float3 Lr = make_float3(0.0f);
		float num_samples = light->get_no_of_samples();
		for (int i = 0; i < num_samples; i++) {
			float3 light_dir, L;
			if (!light->sample(hit.position, light_dir, L)) continue;

			float3 n = hit.shading_normal;
			float3 wi = light_dir;
			float3 wr = optix::reflect(-wi, n);//2 * dot(wi, n) * n - wi;
			float3 wo = -normalize(r.direction);
			Lr += (rho_s * (s + 2) * (0.5f * M_1_PIf) * pow(fmax(0, dot(wo, wr)), s)) * L * fmax(0, dot(wi, n));
		}
		result += (Lr / num_samples);
	}
	return result + Emission::shade(r, hit, emit);
}
