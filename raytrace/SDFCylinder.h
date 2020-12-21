#pragma once


#ifndef SDFCYLINDER_H
#define SDFCYLINDER_H

#include <vector>
#include <optix_world.h>
#include "SDFObject.h"

class SDFCylinder : public SDFObject
{
public:
	SDFCylinder(optix::float3 cent, optix::float3 a, float r)
		: center(cent), axis(a), radius(r)
	{}

	void transform(const optix::Matrix4x4& m);
	float distance(const optix::float3& pos) const;
	virtual void add_to_bbox(optix::Aabb& bb) const;



private:
	float radius;
	optix::float3 center, axis;
};

#endif // SDFCYLINDER_H
