// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011 & Tim Kaiser, 2020
// Copyright (c) DTU Informatics 2011

#ifndef SDF_H
#define SDF_H

#include <vector>
#include <optix_world.h>
#include "../HitInfo.h"
#include "../ObjMaterial.h"
#include "../Object3D.h"
#include"../SDFObject.h"

class SDF : public Object3D
{
public:
	SDF(std::vector<SDFObject*> sdfObjs, ObjMaterial& obj_material) : sdfObjects(sdfObjs), material(obj_material)	{}

	virtual bool intersect(const optix::Ray& ray, HitInfo& hit, unsigned int prim_idx) const;
	virtual void transform(const optix::Matrix4x4& m);
	virtual optix::Aabb compute_bbox() const;

	const ObjMaterial& get_material() const { return material; }

	virtual float distance(const optix::float3& rayPos) const;
	virtual optix::float3 calcNormal(const optix::float3& rayPos) const;

private:
	ObjMaterial material;
	std::vector<SDFObject*> sdfObjects;
	SDFObject* root;

};

#endif // SDF_H