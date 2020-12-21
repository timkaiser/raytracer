#pragma once


#ifndef SDFCSGTREE_H
#define SDFCSGTREE_H

#include <vector>
#include <optix_world.h>
#include "SDFObject.h"
#include "Blending.h"

class SDFCSGTree : public SDFObject
{
public:
	SDFCSGTree(SDFObject* l, SDFObject* r, Blendfunction func, float blendpara)
		: left(l), right(r), blendfunction(func), blendparameter(blendpara)
	{}

	virtual void transform(const optix::Matrix4x4& m);
	float distance(const optix::float3& pos) const;
	virtual void add_to_bbox(optix::Aabb& bb) const;

private:
	SDFObject* left;
	SDFObject* right;
	Blendfunction blendfunction;
	float blendparameter;	//mostly used for smoothmin
};

#endif // SDFCSGTREE_H