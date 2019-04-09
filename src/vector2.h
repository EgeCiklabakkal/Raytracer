#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

class Vec2
{
	public:

	float e[2];

	// Constructors
	Vec2() { e[0] = 0; e[1] = 0; }
	Vec2(float e0, float e1) { e[0] = e0; e[1] = e1; }
	Vec2(const Vec2& v)
	{
		e[0] = v.e[0]; e[1] = v.e[1];
	}

	// Getters
	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float operator[](int i) const { assert(i >= 0 && i < 2); return e[i]; }
	float& operator[](int i) { assert(i >= 0 && i < 2); return e[i]; }

	// Setters
	void setX(float a) { e[0] = a; }
	void setY(float a) { e[1] = a; }

	// Unary Operators
	const Vec2& operator+() const { return *this; }
	Vec2 operator-() const { return Vec2(-e[0], -e[1]); }

	// Boolean Operators
	friend bool operator==(const Vec2& v1, const Vec2& v2);
	friend bool operator!=(const Vec2& v1, const Vec2& v2);

	// I/O Operators
	friend std::istream& operator>>(std::istream& is, Vec2& v);
	friend std::ostream& operator<<(std::ostream& os, const Vec2& v);

	// Arithmetic Operators
	friend Vec2 operator+(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator-(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator/(const Vec2& v, float s);
	friend Vec2 operator*(const Vec2& v, float s);
	friend Vec2 operator*(float s, const Vec2& v);

	// Assignment Operators
	Vec2& operator=(const Vec2& rhs);
	Vec2& operator+=(const Vec2& rhs);
	Vec2& operator-=(const Vec2& rhs);
	Vec2& operator*=(const float s);
	Vec2& operator/=(const float s);

	// Methods
	float length() const { return sqrt(e[0]*e[0] + e[1]*e[1]); }
	float squaredLength() const { return e[0]*e[0] + e[1]*e[1]; }
	void makeUnitVector();

	// Friend Methods
	friend Vec2 unitVector(const Vec2& v);
	friend float dot(const Vec2& v1, const Vec2& v2);
};

inline bool operator==(const Vec2& v1, const Vec2& v2)
{
	return ((v1[0] == v2[0]) && (v1[1] == v2[1]));
}

inline bool operator!=(const Vec2& v1, const Vec2& v2)
{
	return ((v1[0] != v2[0]) || (v1[1] != v2[1]));
}

inline std::istream& operator>>(std::istream& is, Vec2& v)
{
	is >> v[0] >> v[1];
	return is;
}

inline std::ostream& operator<<(std::ostream& os, Vec2& v)
{
	os << '(' << v[0] << " " << v[1] << ')';
	return os;
}

inline Vec2 unitVector(const Vec2& v)
{
	float k = 1.0f / sqrt(v.e[0]*v.e[0] + v.e[1]*v.e[1]);
	return Vec2(v.e[0]*k, v.e[1]*k);
}

inline void Vec2::makeUnitVector()
{
	float k = 1.0f / sqrt(e[0]*e[0] + e[1]*e[1]);
	e[0] *= k; e[1] *= k;
}

inline Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1]);
}

inline Vec2 operator-(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1]);
}

inline Vec2 operator/(const Vec2& v, float s)
{
	return Vec2(v.e[0] / s, v.e[1] / s);
}

inline Vec2 operator*(const Vec2& v, float s)
{
	return Vec2(s * v.e[0], s * v.e[1]);
}

inline Vec2 operator*(float s, const Vec2& v)
{
	return Vec2(s * v.e[0], s * v.e[1]);
}

inline float dot(const Vec2& v1, const Vec2& v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1];
}

inline Vec2& Vec2::operator=(const Vec2 &rhs)
{
	e[0] = rhs.e[0];
	e[1] = rhs.e[1];
	
	return *this;
}

inline Vec2& Vec2::operator+=(const Vec2 &rhs)
{
	e[0] += rhs.e[0];
	e[1] += rhs.e[1];
	
	return *this;
}

inline Vec2& Vec2::operator-=(const Vec2 &rhs)
{
	e[0] -= rhs.e[0];
	e[1] -= rhs.e[1];
	
	return *this;
}
	
inline Vec2& Vec2::operator*=(const float s)
{
	e[0] *= s;
	e[1] *= s;

	return *this;
}

inline Vec2& Vec2::operator/=(const float s)
{
	e[0] /= s;
	e[1] /= s;

	return *this;
}

#endif
