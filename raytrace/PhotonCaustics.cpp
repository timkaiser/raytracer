// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "PhotonCaustics.h"

using namespace optix;

float3 PhotonCaustics::shade(const Ray& r, HitInfo& hit, bool emit) const
{
  float3 rho_d = get_diffuse(hit);

  // Make a radiance estimate using the caustics photon map
  //
  // Input:  r          (the ray that hit the material)
  // x        hit        (info about the ray-surface intersection)
  // x        emit       (passed on to Emission::shade)
  //
  // Return: radiance reflected to where the ray was coming from
  //
  // Relevant data fields that are available (see PhotonCaustics.h and above):
  // rho_d              (diffuse reflectance of the material)
  // x tracer             (pointer to particle tracer)
  // x max_dist           (maximum radius of radiance estimate)
  // x photons            (maximum number of photons to be included in radiance estimate)
  //
  // Hint: Use the function tracer->caustics_irradiance(...) to do an
  //       irradiance estimate using the photon map. This is not the 
  //       same as a radiance estimate.

  //works, (kind of)
  HitInfo hit1;
  hit1.has_hit = hit.has_hit;
  hit1.dist = hit.dist;
  hit1.geometric_normal = hit.geometric_normal;
  hit1.material = hit.material;
  hit1.position = hit.position;
  hit1.ray_ior = hit.ray_ior;
  hit1.shading_normal = hit.shading_normal;
  hit1.texcoord = hit.texcoord;
  hit1.trace_depth = hit.trace_depth;

  float3 L = Lambertian::shade(r, hit1, emit);
  float3 c = rho_d * M_1_PIf * tracer->caustics_irradiance(hit1, max_dist, photons);

  /* Doesn't work (stripes across the screen)
  float3 L = Lambertian::shade(r, hit, emit);
  float3 c = rho_d * M_1_PIf * tracer->caustics_irradiance(hit, max_dist, photons);  
  */

  /*Doesn't work (wierd patterns on the plane)
  float3 c = rho_d * M_1_PIf * tracer->caustics_irradiance(hit1, max_dist, photons);
  float3 L = Lambertian::shade(r, hit1, emit);
  */


  return  L + c;//c.x == 0 && c.y == 0 && c.z == 0 ? make_float3(1,0,0) : c.x < 0.2f && c.y < 0.2f && c.z < 0.2f ? make_float3(0, 1, 0) : make_float3(0, 0, 1);
}
