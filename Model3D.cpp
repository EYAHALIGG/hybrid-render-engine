#include "Model3D.h"

#pragma warning(disable:4996)

Model3D::Model3D(const char * fileName)
{
	std::vector<Vec3f> tempVertices;
	std::vector<Vec3f> tempNormals;
	std::vector<int> tempVertexIndices;
	std::vector<int> tempNormalIndices;
	minPoint = Vec3f(1, 1, 1) * 1e10, maxPoint = -minPoint;
	int vertexIndex[3], uvIndex[3], normalIndex[3];
	FILE* file = fopen(fileName, "r");
	char lineHeader[128];
	while (true)
	{
		if (fscanf(file, "%s", lineHeader) == EOF) { break; }
		Vec3f p;
		if (!strcmp(lineHeader, "v"))
		{
			fscanf(file, "%f %f %f\n", &p.x, &p.y, &p.z);
			tempVertices.push_back(p);
			if (p.x > maxPoint.x) { maxPoint.x = p.x; }
			if (p.y > maxPoint.y) { maxPoint.y = p.y; }
			if (p.z > maxPoint.z) { maxPoint.z = p.z; }
			if (p.x < minPoint.x) { minPoint.x = p.x; }
			if (p.y < minPoint.y) { minPoint.y = p.y; }
			if (p.z < minPoint.z) { minPoint.z = p.z; }
		}
		else if (!strcmp(lineHeader, "vn"))
		{
			fscanf(file, "%f %f %f\n", &p.x, &p.y, &p.z);
			tempNormals.push_back(p);
		}
		else if (!strcmp(lineHeader, "f"))
		{
			fscanf(file, "%d//%d %d//%d %d//%d\n", // "%d//%d//%d %d//%d//%d %d//%d//%d\n",
				&vertexIndex[0]/*, &uvIndex[0]*/, &normalIndex[0],
				&vertexIndex[1]/*, &uvIndex[1]*/, &normalIndex[1],
				&vertexIndex[2]/*, &uvIndex[2]*/, &normalIndex[2]);

			tempVertexIndices.push_back(vertexIndex[0]);
			tempVertexIndices.push_back(vertexIndex[1]);
			tempVertexIndices.push_back(vertexIndex[2]);
			tempNormalIndices.push_back(normalIndex[0]); // all of the normals, which are on the same face, are the same normal therefore reading first one is enough
		}
	}
	size = maxPoint - minPoint;
	centerPoint = (minPoint + maxPoint) * 0.5f;
	pivotPoint = centerPoint; // Auto-Center
	for (int i = 0; i < tempVertexIndices.size(); i += 3)
	{
		Face3D face;
		face.triangle.p1 = tempVertices[tempVertexIndices[i] - 1];
		face.triangle.p2 = tempVertices[tempVertexIndices[i + 1] - 1];
		face.triangle.p3 = tempVertices[tempVertexIndices[i + 2] - 1];
		face.normal = tempNormals[tempNormalIndices[i / 3] - 1];
		faces.push_back(face);
		// TODO texture and material
	}
}

