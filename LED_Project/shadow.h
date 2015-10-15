#pragma once

#include <Windows.h>
#include <vector>
#define MAX_LENGTH 65535
//define the struct of points
struct Point_s{
	float x, y, z;
};
struct PlaneEquation_s{
	float a, b, c, d;
};
typedef struct Plane_s{
	unsigned int p[3];
	glm::vec3 normals[3];
	unsigned int adjFace_index[3];
	PlaneEquation_s planeEq;
	bool visible;
}Face;

struct glObject{
	unsigned int planes_n, points_n;
	//Point_s points[MAX_LENGTH];
	std::vector<glm::vec3> points;
	Plane_s planes[MAX_LENGTH];
};
typedef struct glObject ShadowedObject;
//class ShadowedObject
//{
//public:
//	unsigned int planes_n;
//	unsigned int points_n;
//	//Point_s points[MAX_LENGTH];
//	//	vector<glm::vec3> points;
//	//	Plane_s planes[65535];
//};