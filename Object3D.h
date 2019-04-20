	#pragma once

#include "Vec3.h"

class VoxelMap3D;

class Object3D
{
	VoxelMap3D* voxelMap = 0;
	Vec3f position = Vec3f();
public:
	Object3D(VoxelMap3D* _voxelMap) : voxelMap(_voxelMap) {}
	void SetPosition(Vec3f& _position) { position = _position; }
	Vec3f GetPosition() { return position; }
	VoxelMap3D* GetVoxelMap() { return voxelMap; }
};