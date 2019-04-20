#include "Renderer3D.h"
#include "Object3D.h"
#include "Model3D.h"
#include "VoxelMap3D.h"
#include <chrono>
#include <iostream>
#include <string>

void Renderer3D::GenerateRay(float x, float y, Vec3f & rayOrigin, Vec3f & rayDirection)
{
	rayOrigin = Vec3f(0, 0, -5.0f);
	rayDirection = Vec3f(
		x - (float)screen->GetSize().x * 0.5f,
		y - (float)screen->GetSize().y * 0.5f,
		(float)screen->GetSize().y / tan(45 * 3.14 / 180 / 2.0f)
	).Normalize();
}

Vec3f Renderer3D::RayTracingShader(int x, int y)
{
	Vec3f color = Vec3f();
	Vec3f rayOrigin, rayDirection;
	const float sppsqrt = RAY_TRACING_SAMPLES_PER_PIXEL_SQRT;
	const float invsppsqrt = 1.0f / sppsqrt;
	const float spp = sppsqrt * sppsqrt;
	for(int samplesY = 0; samplesY < sppsqrt; samplesY++)
		for (int samplesX = 0; samplesX < sppsqrt; samplesX++)
		{
			GenerateRay((float)x + (float)samplesX * invsppsqrt, (float)y + (float)samplesY * invsppsqrt, rayOrigin, rayDirection);
			color = color + this->TraceRay(rayOrigin, rayDirection, 0);
		}
	const float invspp = 1.0f / spp;
	return /*GammaCorrection*/(color * invspp); // average sampling
}

Vec3f Renderer3D::TraceRay(Vec3f rayOrigin, Vec3f rayDirection, int depth)
{
	Object3D* object = objects[0]; // ilk baþta sadece bir obje kontrolü var.
	Vec3f voxelRayOrigin, voxelRayDirection;
	if (RayBoundingBoxIntersection(rayOrigin, rayDirection, object, voxelRayOrigin, voxelRayDirection))
	{
		//if (VoxelMaterial3D* material = object->GetVoxelMap()->CastRay(voxelRayOrigin, voxelRayDirection))
		if(VoxelMaterial3D* material = object->GetVoxelMap()->MarchRay(voxelRayOrigin, voxelRayDirection))
			return this->BasicLighting(
				((Vec3f(-5, -1.5f, -10.0f) - // light position on the model space
				object->GetPosition() +
				object->GetVoxelMap()->GetModel()->GetCenterPoint() -
				object->GetVoxelMap()->GetModel()->GetMinPoint()) * object->GetVoxelMap()->GetModel2RamScale() - voxelRayOrigin).Normalize(),
				rayDirection, material->normal);
		//else return Vec3f(1, 0, 0);
	}
	return Vec3f(.45, .45, .45);
}

/*
object->GetVoxelMap()->GetModel2VoxelTransformation() * lightPosition
*/

bool Renderer3D::RayBoundingBoxIntersection(Vec3f & rayOrigin, Vec3f & rayDirection, Object3D * object, Vec3f & intersectionPoint, Vec3f & voxelRayDirection)
{
	float t;
	Vec3f p, pmin;
	float const floatmax = 1e10;
	float tmin = floatmax;
	voxelRayDirection = rayDirection; // rotasyonlar olmadýðý için þimdilik böyle
	Vec3f rayOriginTransformed = 
		rayOrigin -
		object->GetPosition() +
		object->GetVoxelMap()->GetModel()->GetPivotPoint() - //GetCenterPoint() -
		object->GetVoxelMap()->GetModel()->GetMinPoint();
	// Vec3f rayOriginTransformed = rayOrigin * object->GetModel2AABBTransformation();
	Vec3f modelSize = object->GetVoxelMap()->GetModel()->GetSize();
	if (voxelRayDirection.x != 0)
	{	
		t = -rayOriginTransformed.x / voxelRayDirection.x;
		p = rayOriginTransformed + voxelRayDirection * t;
		p.x = 0;
		if (t >= 0 && t < tmin && p.IsInsideBox(modelSize)) { tmin = t; pmin = p; }
		t = (modelSize.x - rayOriginTransformed.x) / voxelRayDirection.x;
		p = rayOriginTransformed + voxelRayDirection * t;
		p.x = modelSize.x;
		if (t >= 0 && t < tmin && p.IsInsideBox(modelSize)) { tmin = t; pmin = p; }
	}
	if (voxelRayDirection.y != 0)
	{
		t = -rayOriginTransformed.y / voxelRayDirection.y;
		p = rayOriginTransformed + voxelRayDirection * t;
		p.y = 0;
		if (t >= 0 && t < tmin && p.IsInsideBox(modelSize)) { tmin = t; pmin = p; }
		t = (modelSize.y - rayOriginTransformed.y) / voxelRayDirection.y;
		p = rayOriginTransformed + voxelRayDirection * t;
		p.y = modelSize.y;
		if (t >= 0 && t < tmin && p.IsInsideBox(modelSize)) { tmin = t; pmin = p; }
	}
	if (voxelRayDirection.z != 0)
	{
		t = -rayOriginTransformed.z / voxelRayDirection.z;
		p = rayOriginTransformed + voxelRayDirection * t;
		p.z = 0;
		if (t >= 0 && t < tmin && p.IsInsideBox(modelSize)) { tmin = t; pmin = p; }
		t = (modelSize.z - rayOriginTransformed.z) / voxelRayDirection.z;
		p = rayOriginTransformed + voxelRayDirection * t;
		p.z = modelSize.z;
		if (t >= 0 && t < tmin && p.IsInsideBox(modelSize)) { tmin = t; pmin = p; }
	}
	intersectionPoint = pmin;
	return tmin != floatmax;
}

Vec3f Renderer3D::BasicLighting(Vec3f& lightDirection, Vec3f& viewDirection, Vec3f & normal)
{
	//return Vec3f(0, 1, 0);
	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	Vec3f objectColor = Vec3f(.5, .5, 0.25);
	Vec3f lightColor = Vec3f(1, 1, 1);
	Vec3f reflectDirection = (-lightDirection).Reflect(normal);
	return objectColor * lightColor *(
		ambientStrength +																// Ambient
		max(normal.Dot(lightDirection), 0) +											// Diffuse
		pow(max(viewDirection.Dot(reflectDirection), 0), 32) * specularStrength);		// Specular
}

Vec3f Renderer3D::GammaCorrection(Vec3f color)
{
	return Vec3f(sqrt(color.x), sqrt(color.y), sqrt(color.z));
}

void Renderer3D::Render()
{
	printf("Timer started...\n");
	auto t1 = std::chrono::high_resolution_clock::now();
	screen->PostProcess(&Renderer3D::RayTracingShader, this);
	auto t2 = std::chrono::high_resolution_clock::now();
	//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;
	//std::cout << objects[0]->GetVoxelMap()->castCounter << std::endl;
	//system("pause");
	std::string str =
		std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) + " ms, " +
		std::to_string(objects[0]->GetVoxelMap()->GetModel()->GetFaces().size()) + " tris, " +
		std::to_string(objects[0]->GetVoxelMap()->castCounter) + " iterations";
	SetWindowText(GetConsoleWindow(), str.c_str());
	system("pause");
	screen->Render();
}
