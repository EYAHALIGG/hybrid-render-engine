#include "Image2D.h"
#include <Windows.h>

void Image2D::Render()
{
	HDC hdc = GetDC(GetConsoleWindow());
	for (int y = 0; y < size.y; y++)
		for (int x = 0; x < size.x; x++)
			SetPixelV(hdc, x, y, pixels[x + y * size.x]);
}
