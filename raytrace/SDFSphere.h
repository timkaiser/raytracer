#pragma once


#ifndef SDFSPHERE_H
#define SDFSPHERE_H

#include <vector>
#include <optix_world.h>
#include "SDFObject.h"

class SDFSphere : public SDFObject
{
public:
	SDFSphere(optix::float3 cent, float rad)
		: center(cent), radius(rad)
	{}

	float distance(optix::float3& pos) const;


private:
	optix::float3 center;
	float radius;
};

#endif // SDFSPHERE_H