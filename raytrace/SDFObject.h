
#ifndef SDFOBJECT_H
#define SDFOBJECT_H

#include <vector>
#include <optix_world.h>

class SDFObject
{
public:
	virtual void transform(const optix::Matrix4x4& m);
	virtual float distance(const optix::float3& rayPos) const;
	virtual void add_to_bbox(optix::Aabb& bb) const;


private:
	optix::float3 position;
	float hitRadius;
};

#endif // SDFOBJECT_H