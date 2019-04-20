#include "VoxelMap3D.h"
#include "Model3D.h"
#include <algorithm>
#include <Windows.h>
#include "Vec2.h"
#include <iostream>

int VoxelMap3D::GetVoxelIndex(Vec3i & position)
{
	return position.x + position.y * (size.x + 1) + position.z * (size.x + 1) * (size.y + 1);
}

void VoxelMap3D::SetVoxelUnsafe(Vec3i & position, VoxelMaterial3D & material)
{
	if (!position.IsInsideBox(this->size)) { return; } // TODO: it can be removed
	unsigned index = GetVoxelIndex(position);
	int* voxelMapElementPtr;
	if (*(voxelMapElementPtr = &voxelMapPtr[index])) { return; }
	voxelMaterials.push_back(material);
	*voxelMapElementPtr = voxelMaterials.size();
	voxelPoints.push_back(Vec3f(position));
	//if (voxelMapPtr[GetVoxelIndex(position)]) { return; }
	//voxelMaterials.push_back(material);
	//voxelMapPtr[GetVoxelIndex(position)] = voxelMaterials.size();
}

VoxelMaterial3D* VoxelMap3D::GetVoxelUnsafe(Vec3i & position)
{
	int materialIndex = voxelMapPtr[GetVoxelIndex(position)];
	if (!materialIndex) { return 0; }
	//if(materialIndex > 0)
	return &voxelMaterials[materialIndex - 1];
	//else return (VoxelMaterial3D*)materialIndex;
}

void VoxelMap3D::FillTriangle(Triangle3D& triangle, VoxelMaterial3D & material)
{
	Vec3f v[3] = { triangle.p1, triangle.p2, triangle.p3 };
	if (triangle.p1.z == triangle.p2.z == triangle.p3.z)
	{
		std::sort(v, v + 3, compy);
		if (v[0].y == v[1].y) { this->FillDividedTriangle(v[2], v[0], v[1], material); }
		else if (v[1].y == v[2].y) { this->FillDividedTriangle(v[0], v[1], v[2], material); }
		else
		{
			float t = (v[1].y - v[0].y) / (v[2].y - v[0].y);
			Vec3f v3 = v[0] + (v[2] - v[0]) * t;
			this->FillDividedTriangle(v[0], v3, v[1], material);
			this->FillDividedTriangle(v[2], v3, v[1], material);
		}
	}
	else
	{
		std::sort(v, v + 3, compz);
		if (v[0].z == v[1].z) { this->FillDividedTriangle(v[2], v[0], v[1], material); }
		else if (v[1].z == v[2].z) { this->FillDividedTriangle(v[0], v[1], v[2], material); }
		else
		{
			float t = (v[1].z - v[0].z) / (v[2].z - v[0].z);
			Vec3f v3 = v[0] + (v[2] - v[0]) * t;
			this->FillDividedTriangle(v[0], v3, v[1], material);
			this->FillDividedTriangle(v[2], v3, v[1], material);
		}
	}
}

void VoxelMap3D::FillDividedTriangle(Vec3f & vPeak, Vec3f & v1, Vec3f & v2, VoxelMaterial3D & material)
{
	int imax = sqrt(max((v1 - vPeak).SqrLength(), (v2 - vPeak).SqrLength())) * model2RamScale;
	for (int i = 0; i <= imax; i++)
	{
		Vec3f left = vPeak + ((v1 - vPeak) / (float)imax) * i;
		Vec3f right = vPeak + ((v2 - vPeak) / (float)imax) * i;
		this->DrawLine(left, right, material);
	}
}

void VoxelMap3D::DrawLine(Vec3f p1, Vec3f p2, VoxelMaterial3D & material) // TODO hatalý
{
	/*int imax = (p2 - p1).Length() * model2RamScale;
	if (!imax) { return; }
	p1 = p1 - GetModel()->GetMinPoint();
	p2 = p2 - GetModel()->GetMinPoint();
	for (int i = 0; i <= imax; i++)
		this->SetVoxelUnsafe(Vec3i(p1 + ((p2 - p1) / (float)imax * i * model2RamScale)), material);*/
	p1 = (p1 - GetModel()->GetMinPoint()) * model2RamScale;
	p2 = (p2 - GetModel()->GetMinPoint()) * model2RamScale;
	if ((p1.x == p2.x) && (p1.y == p2.y) && (p1.z == p2.z)) { return this->SetVoxelUnsafe(Vec3i(p1), material); }
	int distance = (p1 - p2).Length();
	for (int i = 0; i <= distance; i++)
		this->SetVoxelUnsafe(Vec3i(p2 + (p1 - p2).Normalize() * i), material);
}

void VoxelMap3D::FillTriangle4Normal(Triangle3D & triangle, VoxelMaterial3D & material)
{
	Vec3f primMin = Vec3f(
		min(min(triangle.p1.x, triangle.p2.x), triangle.p3.x),
		min(min(triangle.p1.y, triangle.p2.y), triangle.p3.y),
		min(min(triangle.p1.z, triangle.p2.z), triangle.p3.z));
	Vec3f primMax = Vec3f(
		max(max(triangle.p1.x, triangle.p2.x), triangle.p3.x),
		max(max(triangle.p1.y, triangle.p2.y), triangle.p3.y),
		max(max(triangle.p1.z, triangle.p2.z), triangle.p3.z));
	primMin = (primMin - this->GetModel()->GetMinPoint()) * model2RamScale;
	primMax = (primMax - this->GetModel()->GetMinPoint()) * model2RamScale;
	Triangle3D voxPos;
	voxPos.p1 = (triangle.p1 - this->GetModel()->GetMinPoint()) * model2RamScale;
	voxPos.p2 = (triangle.p2 - this->GetModel()->GetMinPoint()) * model2RamScale;
	voxPos.p3 = (triangle.p3 - this->GetModel()->GetMinPoint()) * model2RamScale;
	for (int z = primMin.z; z <= primMax.z; z++)
		for (int y = primMin.y; y <= primMax.y; y++)
			for (int x = primMin.x; x <= primMax.x; x++)
				if (this->EdgeVoxelTest(Vec3f(x, y, z), voxPos))
					this->SetVoxelUnsafe(Vec3i(x, y, z), material);
}

bool VoxelMap3D::EdgeVoxelTest(Vec3f & voxelCenter, Triangle3D & triangle)
{
	Vec3f e0 = triangle.p2 - triangle.p1;
	Vec3f e1 = triangle.p3 - triangle.p2;
	Vec3f e2 = triangle.p1 - triangle.p3;
	Vec3f planeNormal = e0.Cross(e1);
	const float voxelExtentH = 1;
	// XY Plane Projection
	{
		float isFront = -SIGN(planeNormal.z);//planeNormal.z < 0 ? 1 : (planeNormal.z > 0 ? -1 : 0);
		Vec2f eNrm0 = Vec2f(e0.y, -e0.x) * isFront;
		Vec2f eNrm1 = Vec2f(e1.y, -e1.x) * isFront;
		Vec2f eNrm2 = Vec2f(e2.y, -e2.x) * isFront;
		Vec2f an0 = Vec2f(abs(eNrm0.x), abs(eNrm0.y));
		Vec2f an1 = Vec2f(abs(eNrm1.x), abs(eNrm1.y));
		Vec2f an2 = Vec2f(abs(eNrm2.x), abs(eNrm2.y));
		Vec3f eOfs = Vec3f(
			an0.x + an0.y,
			an1.x + an1.y,
			an2.x + an2.y
		) * voxelExtentH;
		Vec3f ef = eOfs - Vec3f(
			Vec2f(Vec2f(triangle.p1.x, triangle.p1.y) - Vec2f(voxelCenter.x, voxelCenter.y)).Dot(eNrm0),
			Vec2f(Vec2f(triangle.p2.x, triangle.p2.y) - Vec2f(voxelCenter.x, voxelCenter.y)).Dot(eNrm1),
			Vec2f(Vec2f(triangle.p3.x, triangle.p3.y) - Vec2f(voxelCenter.x, voxelCenter.y)).Dot(eNrm2)
		);
		if (ef.x < 0 || ef.y < 0 || ef.z < 0) { return false; }
	}
	// YZ Plane Projection
	{
		float isFront = -SIGN(planeNormal.x);
		Vec2f eNrm0 = Vec2f(e0.z, -e0.y) * isFront;
		Vec2f eNrm1 = Vec2f(e1.z, -e1.y) * isFront;
		Vec2f eNrm2 = Vec2f(e2.z, -e2.y) * isFront;
		Vec2f an0 = Vec2f(abs(eNrm0.x), abs(eNrm0.y));
		Vec2f an1 = Vec2f(abs(eNrm1.x), abs(eNrm1.y));
		Vec2f an2 = Vec2f(abs(eNrm2.x), abs(eNrm2.y));
		Vec3f eOfs = Vec3f(
			an0.x + an0.y,
			an1.x + an1.y,
			an2.x + an2.y
		) * voxelExtentH;
		Vec3f ef = eOfs - Vec3f(
			Vec2f(Vec2f(triangle.p1.y, triangle.p1.z) - Vec2f(voxelCenter.y, voxelCenter.z)).Dot(eNrm0),
			Vec2f(Vec2f(triangle.p2.y, triangle.p2.z) - Vec2f(voxelCenter.y, voxelCenter.z)).Dot(eNrm1),
			Vec2f(Vec2f(triangle.p3.y, triangle.p3.z) - Vec2f(voxelCenter.y, voxelCenter.z)).Dot(eNrm2)
		);
		if (ef.x < 0 || ef.y < 0 || ef.z < 0) { return false; }
	}
	// ZX Plane Projection
	{
		float isFront = -SIGN(planeNormal.y);
		Vec2f eNrm0 = Vec2f(e0.x, -e0.z) * isFront;
		Vec2f eNrm1 = Vec2f(e1.x, -e1.z) * isFront;
		Vec2f eNrm2 = Vec2f(e2.x, -e2.z) * isFront;
		Vec2f an0 = Vec2f(abs(eNrm0.x), abs(eNrm0.y));
		Vec2f an1 = Vec2f(abs(eNrm1.x), abs(eNrm1.y));
		Vec2f an2 = Vec2f(abs(eNrm2.x), abs(eNrm2.y));
		Vec3f eOfs = Vec3f(
			an0.x + an0.y,
			an1.x + an1.y,
			an2.x + an2.y
		) * voxelExtentH;
		Vec3f ef = eOfs - Vec3f(
			Vec2f(Vec2f(triangle.p1.z, triangle.p1.x) - Vec2f(voxelCenter.z, voxelCenter.x)).Dot(eNrm0),
			Vec2f(Vec2f(triangle.p2.z, triangle.p2.x) - Vec2f(voxelCenter.z, voxelCenter.x)).Dot(eNrm1),
			Vec2f(Vec2f(triangle.p3.z, triangle.p3.x) - Vec2f(voxelCenter.z, voxelCenter.x)).Dot(eNrm2)
		);
		if (ef.x < 0 || ef.y < 0 || ef.z < 0) { return false; }
	}
	return true;
}

void VoxelMap3D::PrepareDistanceMapping()
{
	for (int z = 0; z <= this->size.z; z++)
		for (int y = 0; y <= this->size.y; y++)
			for (int x = 0; x <= this->size.x; x++)
			{
				if (!voxelMapPtr[GetVoxelIndex(Vec3i(x, y, z))])
				{
					//voxelMapPtr[GetVoxelIndex(Vec3i(x, y, z))] = -1; continue;
					float mindis = 1e10;
					float d;
					for (int i = 0; i < voxelPoints.size(); i++)
					{
						d = (Vec3f(x, y, z) - voxelPoints[i]).SqrLength();
						if (d < mindis) { mindis = d; }
					}
					mindis = sqrt(mindis);
					int di = mindis;
					if (di == 0) { di = 1; }
					voxelMapPtr[GetVoxelIndex(Vec3i(x, y, z))] = -di;
					//int zaaaaa = 0;
					//printf("%d ", -di);
				}
			}
	//Sleep(10000);
}

VoxelMap3D::VoxelMap3D(Model3D * _model)
{
	model = _model;
	const int maxVoxelAxisUnit = 30; // örneðin bu deðer 300 ise, voxel map boyutu 250 x '300' x 280 olabilir. Yani 3d model, voxel haritanýn max olan kenarý bu deðer olacak þekilde içine sýðdýrýlýyor.
	model2RamScale = (float)maxVoxelAxisUnit / max(max(model->GetSize().x, model->GetSize().y), model->GetSize().z);
	this->size = model->GetSize() * (float)model2RamScale;
	this->voxelMapPtr = new int[(this->size + Vec3i(1, 1, 1)).Volume()]();
	std::vector<Face3D> faces = this->model->GetFaces();
	for (int i = 0; i < faces.size(); i++)
	{
		VoxelMaterial3D material;
		material.normal = faces[i].normal;
		FillTriangle4Normal(faces[i].triangle, material);
		///material.normal = Vec3f(0, -1, 0);
		///for (int y = 0; y <= this->GetSize().z; y++)
		///	for (int x = 0; x <= this->GetSize().y; x++)
		///		SetVoxelUnsafe(Vec3i(0, x, y), material);
	}
	this->PrepareDistanceMapping();

	/*for (int z = 0; z <= size.z; z++)
		for (int y = 0; y <= size.y; y++)
			for (int x = 0; x <= size.x; x++)
				if (((int)(Vec3f(x, y, z) - Vec3f(size) * 0.5f).Length()) <= (int)(size.x * 0.5f))
				{
					VoxelMaterial3D material; material.normal = (Vec3f(x, y, z) - Vec3f(size.x, size.y, size.z) * 0.5f).Normalize();
					SetVoxelUnsafe(Vec3i(x, y, z), material);
				}*/
}

VoxelMaterial3D * VoxelMap3D::CastRay(Vec3f & voxelRayOrigin, Vec3f & voxelRayDirection)
{
	voxelRayOrigin = voxelRayOrigin * this->model2RamScale;
	while (true)
	{
		castCounter++;
		if (!Vec3i(voxelRayOrigin).IsInsideBox(this->GetSize())) { break; }
		if (VoxelMaterial3D* material = GetVoxelUnsafe(Vec3i(voxelRayOrigin))) { return material; }
		voxelRayOrigin = voxelRayOrigin + voxelRayDirection;
	}
	return 0;
}

VoxelMaterial3D * VoxelMap3D::MarchRay(Vec3f & voxelRayOrigin, Vec3f & voxelRayDirection)
{
	voxelRayOrigin = voxelRayOrigin * this->model2RamScale;
	while (true)
	{
		castCounter++;
		if (!Vec3i(voxelRayOrigin).IsInsideBox(this->GetSize())) { break; }
		int a = voxelMapPtr[GetVoxelIndex(Vec3i(voxelRayOrigin))];
		if (a < 0) { voxelRayOrigin = voxelRayOrigin + voxelRayDirection * (float)(-a); }
		else return &voxelMaterials[a - 1];
		/*if (VoxelMaterial3D* material = GetVoxelUnsafe(Vec3i(voxelRayOrigin)))
		{
			if ((int)material > 0)
				return material;
			else voxelRayOrigin = voxelRayOrigin + voxelRayDirection * -(int)material;
		}*/
	}
	return 0;
}
