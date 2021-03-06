#include <fstream>
#include <optix_world.h>
#include "sampler.h"
#include "BRDF.h"

using namespace std;
using namespace optix;

// Rotate vector around an axis
float3 rotate_vector(const float3& vector, const float3& axis, float angle)
{
  // Implement quaternion-based rotation of a vector by an angle around an axis
	return cos(angle) * vector + sin(angle) * cross(axis, vector) + (1 - cos(angle)) * dot(axis, vector) * axis;
}

// Convert vectors in tangent space to half/difference coordinates
void vectors_to_half_diff_coords(const float3& in, const float3& out,
  float& theta_half, float& phi_half, float& theta_diff, float& phi_diff)
{
  // compute halfway vector
	float3 h = normalize(in + out);
  // compute theta_half, phi_half
	theta_half = acos(h.z);
	phi_half = atan2(h.y, h.x);
  // compute diff vector
	float3 d = rotate_vector(in, make_float3(0, 0, 1), -phi_half);
	d = rotate_vector(d, make_float3(0, 1, 0), -theta_half);
  // compute theta_diff, phi_diff	
	theta_diff = acos(d.z);
	phi_diff = atan2(d.y, d.x);
}

float3 lookup_brdf_val(const float* brdf, const float3& n, const float3& normalized_wi, const float3& normalized_wo)
{
  // Transform vectors to tangent space and use the vectors_to_half_diff_coords function
  // to get input for the other version of the lookup_brdf_val function.
  // Hint: There is an onb function for creating an orthonormal basis and a Matrix3x3 data type useful for storing a change of basis matrix.
	float3 b1 = make_float3(0);
	float3 b2 = make_float3(0);
	
	onb(n, b1, b2);
	
	Matrix3x3 m = Matrix3x3();
	m.setRow(0, b1);
	m.setRow(1, b2);
	m.setRow(2, n);

	float3 i = m*normalized_wi;
	float3 o = m * normalized_wo;

	float theta_half, phi_half, theta_diff, phi_diff;
	vectors_to_half_diff_coords(i, o, theta_half, phi_half, theta_diff, phi_diff);

	return lookup_brdf_val(brdf, theta_half, phi_half, theta_diff, phi_diff);
}

float3 integrate_brdf(float* brdf, unsigned int N)
{
  float3 sum = make_float3(0.0f);
  float3 n = make_float3(0.0f, 0.0f, 1.0f);

  // Implement Monte Carlo integration to estimate the bihemisphercial diffuse reflectance (rho).
  // Use N as the number of samples.
  // Hint: Use the functions sample_cosine_weighted and lookup_brdf_val.

  for (int i = 0; i < N; i++) {
	  sum += lookup_brdf_val(brdf, n, normalize(sample_cosine_weighted(n)), normalize(sample_cosine_weighted(n)));
  }
  
  return sum * M_PIf / N;
}

// Read BRDF data
bool read_brdf(const char *filename, float* brdf, unsigned int size, float3& rho_d)
{
  FILE* f = 0;
  if(fopen_s(&f, filename, "rb"))
    return false;

  int dims[3];
  fread(dims, sizeof(int), 3, f);
  unsigned int n = 3*dims[0]*dims[1]*dims[2];
  if(size != n)
    return false;

  double* brdf_d = new double[n];
  fread(&brdf_d[0], sizeof(double), n, f);
  for(unsigned int i = 0; i < size; ++i)
    brdf[i] = static_cast<float>(brdf_d[i]);
  fclose(f);
  delete[] brdf_d;

  rho_d = integrate_brdf(brdf, 100000);
  return true;
}
