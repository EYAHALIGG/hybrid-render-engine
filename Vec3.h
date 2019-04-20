#pragma once

#define VEC3_COMPARATOR(op) \
bool operator op (Vec3& v) {return x op v.x && y op v.y && z op v.z;}

template <typename T>
class Vec3
{
public:
	T x, y, z;
	Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	Vec3() : x(0), y(0), z(0) {}
	template <typename T2> Vec3<T>(Vec3<T2>& v) : x(v.x), y(v.y), z(v.z) {}

	Vec3 operator+(Vec3& v) { return Vec3(x + v.x, y + v.y, z + v.z); }
	Vec3 operator-(Vec3& v) { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3 operator*(Vec3& v) { return Vec3(x * v.x, y * v.y, z * v.z); }
	Vec3 operator/(Vec3& v) { return Vec3(x / v.x, y / v.y, z / v.z); }

	VEC3_COMPARATOR(>);
	VEC3_COMPARATOR(>= );
	VEC3_COMPARATOR(<);
	VEC3_COMPARATOR(<= );

	Vec3 operator-() { return Vec3(-x, -y, -z); }

	Vec3<T> operator*(T val) { return Vec3<T>(x * val, y * val, z * val); } // scalar mul
	Vec3<T> operator/ (T val) { /*if (val == 0) { float za = x / val; printf("%f %f %f\n", za + 5, y / val, z / val); system("pause"); }*/  return Vec3<T>(x / val, y / val, z / val); } // scalar div

	float Dot(Vec3& v) { return x * v.x + y * v.y + z * v.z; }
	Vec3<T> Cross(Vec3& v) { return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

	float SqrLength() { return Dot(*this); }
	float Length() { return sqrt(SqrLength()); }
	Vec3 Normalize() { return *this / Length(); }
	T Volume() { return x * y * z; }

	Vec3 Reflect(Vec3& n) { return *this - n * 2.0f * this->Dot(n); } // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/reflect.xhtml
	//Vec3 Rotate(Vec3& r) { return Mat4x4::Rotate(r) * *this; }

	bool IsInsideBox(Vec3<T>& boxSize) { return x >= 0 && y >= 0 && z >= 0 && x <= boxSize.x && y <= boxSize.y && z <= boxSize.z; }
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;