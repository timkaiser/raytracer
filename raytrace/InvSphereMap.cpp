#include <optix_world.h>
#include "InvSphereMap.h"

using namespace optix;

void InvSphereMap::project_direction(const float3& d, float& u, float& v) const
{
	// Implement an inverse sphere map here.
	//float f = 0.5f / (sqrt(2) * sqrt(d.z + 1)) * 4;
	//	u = d.x * f;// acos(d.z)* M_1_PIf;
	//v = d.y * f;// atan2(d.y, d.x) * 0.5f * M_1_PIf;

	u = acos(d.z) / (M_PIf);
	v = atan2(d.y, d.x) / (2 * M_PIf);

}