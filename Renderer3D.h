#pragma once

class Object3D;

#include "Image2D.h"
#include <vector>

#define RAY_TRACING_SAMPLES_PER_PIXEL_SQRT			1

class Renderer3D
{
	std::vector<Object3D*> objects;
	Image2D* screen;
	void GenerateRay(float x, float y, Vec3f& rayOrigin, Vec3f& rayDirection);
	Vec3f RayTracingShader(int x, int y);
	Vec3f TraceRay(Vec3f rayOrigin, Vec3f rayDirection, int depth);
	bool RayBoundingBoxIntersection(Vec3f& rayOrigin, Vec3f& rayDirection, Object3D* object, Vec3f& intersectionPoint, Vec3f& voxelRayDirection);
	Vec3f BasicLighting(Vec3f& lightDirection, Vec3f& viewDirection, Vec3f& normal);
	Vec3f GammaCorrection(Vec3f color);
public:
	Renderer3D() { screen = new Image2D(Vec2i(500, 500)); }
	void AddObject(Object3D* object) { objects.push_back(object); }
	void Render();
};

/*
ilk hedef, rotasyonlarýn ve hareketli kameranýn olmadýðý tek bir noktadan çekilen görüntü almak.
*/