#pragma once


#ifndef SDFCSGTREE_H
#define SDFCSGTREE_H

#include <vector>
#include <optix_world.h>
#include "SDFObject.h"
#include "Blending.h"
//#include "../ObjMaterial.h"

class SDFCSGTree : public SDFObject
{
public:
	SDFCSGTree(SDFObject* l, SDFObject* r, Blendfunction func, float blendpara)
		: left(l), right(r), blendfunction(func), blendparameter(blendpara)
	{}

	//virtual bool intersect(const optix::Ray& ray, HitInfo& hit, unsigned int prim_idx) const;
	virtual void transform(const optix::Matrix4x4& m);
	//virtual optix::Aabb compute_bbox() const;

	float distance(const optix::float3& pos) const;


private:
	SDFObject* left;
	SDFObject* right;
	Blendfunction blendfunction;
	float blendparameter;
	//mostly used for smoothmin
};

#endif // SDFCSGTREE_H