#pragma once

#include "Vec3.h"
#include "Face3D.h"
#include <vector>

class Model3D
{
	Vec3f minPoint, maxPoint, size, pivotPoint, centerPoint;
	std::vector<Face3D> faces;
public:
	Model3D(const char* fileName);
	Vec3f GetMinPoint() { return minPoint; }
	Vec3f GetMaxPoint() { return maxPoint; }
	Vec3f GetSize() { return size; }
	Vec3f GetCenterPoint() { return centerPoint; }
	void SetPivotPoint(Vec3f& point) { pivotPoint = point; }
	Vec3f GetPivotPoint() { return pivotPoint; }
	std::vector<Face3D> GetFaces() { return faces; }
};