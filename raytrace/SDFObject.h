
#ifndef SDFOBJECT_H
#define SDFOBJECT_H

#include <vector>
#include <optix_world.h>
//#include "../ObjMaterial.h"

class SDFObject
{
public:
	virtual void transform(const optix::Matrix4x4& m);
	virtual float distance(const optix::float3& rayPos) const;


private:
	optix::float3 position;
	float hitRadius;
	//ObjMaterial material;
	//std::vector<Object3D*> objects;
};

#endif // SDFOBJECT_H