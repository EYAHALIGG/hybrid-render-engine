#include "Matrix3D.h"
#include "Vec3.h"
#include <math.h>

Matrix3D Matrix3D::operator*(Matrix3D & mat)
{
	Matrix3D ret;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float sum = 0;
			for (int k = 0; k < 4; k++)
				sum += m[i][k] * mat.m[k][j];
			ret.m[i][j] = sum;
		}
	}
	return ret;
}

Vec3f Matrix3D::operator*(Vec3f & v)
{
	float v4[4] = { v.x, v.y, v.z, 1.0f };
	float v4ret[4];
	for (int i = 0; i < 4; i++)
	{
		float sum = 0;
		for (int j = 0; j < 4; j++)
			sum += m[i][j] * v4[j];
		v4ret[i] = sum;
	}
	return Vec3f(v4ret[0], v4ret[1], v4ret[2]);
}

Matrix3D Matrix3D::Identity()
{
	Matrix3D mat;
	for (int i = 0; i < 4; i++)
		for (int k = 0; k < 4; k++)
			mat.m[i][k] = 0;
	for (int i = 0; i < 4; i++)
		mat.m[i][i] = 1;
	return mat;
}

Matrix3D Matrix3D::Translate(Vec3f & t)
{
	Matrix3D mat = Matrix3D::Identity();
	mat.m[3][0] = t.x;
	mat.m[3][1] = t.y;
	mat.m[3][2] = t.z;
	return mat;
}

Matrix3D Matrix3D::Rotate(Vec3f & r)
{
	return Matrix3D::RotateX(r.x) * Matrix3D::RotateY(r.y) * Matrix3D::RotateZ(r.z);
}

Matrix3D Matrix3D::RotateX(float r)
{
	Matrix3D mat = Matrix3D::Identity();
	float s = sin(r);
	float c = cos(r);
	mat.m[1][1] = c;
	mat.m[2][1] = s;
	mat.m[1][2] = -s;
	mat.m[2][2] = c;
	return mat;
}

Matrix3D Matrix3D::RotateY(float r)
{
	Matrix3D mat = Matrix3D::Identity();
	float s = sin(r);
	float c = cos(r);
	mat.m[0][0] = c;
	mat.m[2][0] = -s;
	mat.m[0][2] = s;
	mat.m[2][2] = c;
	return mat;
}

Matrix3D Matrix3D::RotateZ(float r)
{
	Matrix3D mat = Matrix3D::Identity();
	float s = sin(r);
	float c = cos(r);
	mat.m[0][0] = c;
	mat.m[1][0] = s;
	mat.m[0][1] = -s;
	mat.m[1][1] = c;
	return mat;
}

Matrix3D Matrix3D::Scale(Vec3f & s)
{
	Matrix3D mat = Matrix3D::Identity();
	mat.m[0][0] = s.x;
	mat.m[1][1] = s.y;
	mat.m[2][2] = s.z;
	return mat;
}