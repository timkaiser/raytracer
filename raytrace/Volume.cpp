// 02576 Rendering Framework
// Written by Jeppe Revall Frisvad, 2010
// Copyright (c) DTU Informatics 2010

#include <optix_world.h>
#include "HitInfo.h"
#include "Volume.h"

using namespace optix;

float3 Volume::shade(const Ray& r, HitInfo& hit, bool emit) const
{
	// If inside the volume, Find the direct transmission through the volume by using
	// the transmittance to modify the result from the Transparent shader.
	if (dot(hit.shading_normal, r.direction) > 0) {
		return Transparent::shade(r, hit, emit) * get_transmittance(hit);
	}
	else {
		return Transparent::shade(r, hit, emit);
	}
}

float3 Volume::get_transmittance(const HitInfo& hit) const
{	

	if (hit.material)
	{
		// Compute and return the transmittance using the diffuse reflectance of the material.
		// Diffuse reflectance rho_d does not make sense for a specular material, so we can use 
		// this material property as an absorption coefficient. Since absorption has an effect
		// opposite that of reflection, using 1/rho_d-1 makes it more intuitive for the user.
		float3 rho_d = make_float3(hit.material->diffuse[0], hit.material->diffuse[1], hit.material->diffuse[2]);
		float3 result = make_float3(
			(rho_d.x != 0) ? exp((-1 / rho_d.x - 1)*hit.dist) : 0.0f,
			(rho_d.y != 0) ? exp((-1 / rho_d.y - 1)*hit.dist) : 0.0f,
			(rho_d.z != 0) ? exp((-1 / rho_d.z - 1)*hit.dist) : 0.0f
		);

		return result;
  }
  return make_float3(1.0f);
}
