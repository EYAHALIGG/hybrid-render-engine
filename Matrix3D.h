#pragma once

template <typename T> class Vec3;
typedef Vec3<float> Vec3f;

class Matrix3D
{
public:
	float m[4][4];
	Matrix3D operator* (Matrix3D& mat);
	Vec3f operator* (Vec3f& v);
	static Matrix3D Identity();
	static Matrix3D Translate(Vec3f& t);
	static Matrix3D Rotate(Vec3f& r);
	static Matrix3D RotateX(float r);
	static Matrix3D RotateY(float r);
	static Matrix3D RotateZ(float r);
	static Matrix3D Scale(Vec3f& s);
};