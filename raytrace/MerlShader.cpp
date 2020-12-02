// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "mt_random.h"
#include "sampler.h"
#include "HitInfo.h"
#include "ObjMaterial.h"
#include "MerlTexture.h"
#include "MerlShader.h"

using namespace optix;

#ifndef M_1_PIf
#define M_1_PIf 0.31830988618379067154
#endif

float3 MerlShader::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  if(hit.trace_depth >= max_depth)
    return make_float3(0.0f);

  const ObjMaterial* m = hit.material;
  MerlTexture* tex = brdfs && m && m->has_texture ? (*brdfs)[m->tex_name] : 0;
  float3 rho_d = get_diffuse(hit);
  float3 result = make_float3(0.0f);

  if (tex == 0 || !tex->has_texture()) { return make_float3(1,0,1); }
  // Implement a path tracing shader here.
  //
  // Input:  r          (the ray that hit the material)
  //         hit        (info about the ray-surface intersection)
  //         emit       (passed on to Emission::shade)
  //
  // Return: radiance reflected to where the ray was coming from
  //
  // Relevant data fields that are available:
  // tracer             (pointer to ray tracer)
  //
  // Hint: Use the function tex->brdf_lookup(...) to retrieve the value of
  //       the measured BRDF for a given light-view configuration. Ensure
  //       that tex is non-zero and has texture before using it.

  float3 dir, L;
  for (Light* light : lights) {
	  float3 resultL = make_float3(0);
	  for (int i = 0; i < light->get_no_of_samples(); i++) {
		  if (light->sample(hit.position, dir, L)) {
			  resultL += tex->brdf_lookup(hit.shading_normal, normalize(-r.direction), normalize(dir)) * L * fmax(0.0f, dot(hit.shading_normal, dir));
		  }
	  }

	  result += resultL / light->get_no_of_samples();
  }

  float pdf = (rho_d.x + rho_d.y + rho_d.z) / 3;
  if (mt_random() < pdf) {
	Ray rayTemp = Ray(hit.position, normalize(sample_cosine_weighted(hit.shading_normal)), 0, 0.001f, RT_DEFAULT_MAX);
	HitInfo hitTemp;
	hitTemp.trace_depth = hit.trace_depth + 1;
	tracer->trace_to_closest(rayTemp, hitTemp);
	result += tex->brdf_lookup(hit.shading_normal, normalize(-r.direction), rayTemp.direction) * M_PIf * shade_new_ray(rayTemp, hitTemp, false) / pdf;
  }
  return result;
}
