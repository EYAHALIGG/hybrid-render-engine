#pragma once

template <typename T>
class Vec2
{
public:
	T x, y;
	Vec2(T _x, T _y) : x(_x), y(_y) {}
	Vec2() : x(0), y(0) {}

	Vec2 operator+(Vec2& v) { return Vec2(x + v.x, y + v.y); }
	Vec2 operator-(Vec2& v) { return Vec2(x - v.x, y - v.y); }
	Vec2 operator*(Vec2& v) { return Vec2(x * v.x, y * v.y); }
	Vec2 operator/(Vec2& v) { return Vec2(x / v.x, y / v.y); }

	Vec2 operator-() { return Vec2(-x, -y); }

	Vec2<T> operator*(T val) { return Vec2<T>(x * val, y * val); } // scalar mul
	Vec2<T> operator/ (T val) { return Vec2<T>(x / val, y / val); } // scalar div

	float Dot(Vec2& v) { return x * v.x + y * v.y; }
	float Cross(Vec2& v) { return x * v.y - y * v.x; } // cross product

	float SqrLength() { return Dot(*this); }
	float Length() { return sqrt(SqrLength()); }
	Vec2 Norm() { return *this / Length(); } // normalize
	T Area() { return x * y; }

											 // cw points
	Vec2 Perp() { return Vec2(y, -x); }

	Vec2 Rotate(float ang) { float s = sin(ang); float c = cos(ang); return Vec2(x*c - y*s, x*s + y*c); } // rotate point around origin

	static Vec2 Angle(float ang) { return Vec2<float>(cos(ang), sin(ang)); }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;