#pragma once


#ifndef SDFBOX_H
#define SDFBOX_H

#include <vector>
#include <optix_world.h>
#include "SDFObject.h"

class SDFBox: public SDFObject
{
public:
	SDFBox(optix::float3 cent, float w, float h, float l)
		: center(cent)
	{
		bounds = optix::make_float3(w, h, l);
	}

	void transform(const optix::Matrix4x4& m);
	float distance(const optix::float3& pos) const;
	virtual void add_to_bbox(optix::Aabb& bb) const;



private:
	optix::float3 bounds;
	optix::float3 center;
};

#endif // SDFBOX_H