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

class SDF : public Object3D
{
public:
	SDF(const optix::float3& sphere_center, float sphere_radius, const ObjMaterial& obj_material)
		: center(sphere_center), radius(sphere_radius), material(obj_material)
	{ }

	virtual bool intersect(const optix::Ray& ray, HitInfo& hit, unsigned int prim_idx) const;
	virtual void transform(const optix::Matrix4x4& m);
	virtual optix::Aabb compute_bbox() const;

	const ObjMaterial& get_material() const { return material; }

	virtual float distSphere(const optix::float3& rayPos, const optix::float3 shperePos, const float radius) const;


private:
	optix::float3 center;
	float radius;
	ObjMaterial material;

};

#endif // SDF_H