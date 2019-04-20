#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include <Windows.h>

class Image2D
{
	int* pixels = 0;
	Vec2i size;
public:
	Image2D(Vec2i _size) : size(_size) { pixels = new int[size.Area()]; }
	Vec2i GetSize() { return size; }
	void SetPixelUnsafe(int x, int y, int color) { pixels[x + y * size.x] = color; }
	template <class T> void PostProcess(Vec3f(T::*func)(int x, int y), T* base);
	void Render();
};


template<class T>
inline void Image2D::PostProcess(Vec3f(T::* func)(int x, int y), T * base)
{
	for (int y = 0; y < size.y; y++)
		for (int x = 0; x < size.x; x++)
		{
			Vec3f color = (base->*func)(x, y);
			this->SetPixelUnsafe(x, y, RGB(color.x * 255.0f, color.y * 255.0f, color.z * 255.0f));
		}
}