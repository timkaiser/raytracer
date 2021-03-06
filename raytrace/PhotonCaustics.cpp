// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "PhotonCaustics.h"

using namespace optix;

float3 PhotonCaustics::shade(const Ray& r, HitInfo& hit, bool emit) const
{
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

    return Lambertian::shade(r, hit, emit) + get_diffuse(hit) * M_1_PIf * tracer->caustics_irradiance(hit, max_dist, photons);
}
