#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <math.h>
#include <iostream>

class Vec3
{
	public:

	// Member variables	
	float e[3];

	// Constructors
	Vec3(float p0=0.0f, float p1=0.0f, float p2=0.0f);
	Vec3(const Vec3& v) { *this = v; }

	// Getters
	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }

	// Setters
	void setX(float x) { e[0] = x; }
	void setY(float y) { e[1] = y; }
	void setZ(float z) { e[2] = z; }

	// Unary Operators
	const Vec3& operator+() const;
	Vec3 operator-() const;
	float operator[](int i) const { return e[i]; }
	float& operator[](int i) { return e[i]; }

	// Methods
	float length() const;
	float squaredLength() const;
	void makeUnitVector();
	float minComponent() const;
	float maxComponent() const;
	float maxAbsComponent() const;
	float minAbsComponent() const;
	int indexOfMinComponent() const;
	int indexOfMinAbsComponent() const;
	int indexOfMaxComponent() const;
	int indexOfMaxAbsComponent() const;

	// Boolean Operators
	friend bool operator==(const Vec3& v1, const Vec3& v2);
	friend bool operator!=(const Vec3& v1, const Vec3& v2);

	// I/O Operators
	friend std::istream& operator>>(std::istream& is, Vec3& v);
	friend std::ostream& operator<<(std::ostream& os, const Vec3& v);

	// Arithmetic Operators
	friend Vec3 operator+(const Vec3& v1, const Vec3& v2);
	friend Vec3 operator-(const Vec3& v1, const Vec3& v2);
	friend Vec3 operator/(const Vec3& v, float s);
	friend Vec3 operator*(const Vec3& v, float s);
	friend Vec3 operator*(float s, const Vec3& v);

	// Assignment Operators
	Vec3& operator=(const Vec3& rhs);
	Vec3& operator+=(const Vec3& rhs);
	Vec3& operator-=(const Vec3& rhs);
	Vec3& operator*=(const float s);
	Vec3& operator/=(const float s);

	// Friend Methods
	friend Vec3 unitVector(const Vec3& v);
	friend Vec3 minVec(const Vec3& v1, const Vec3& v2);
	friend Vec3 maxVec(const Vec3& v1, const Vec3& v2);
	friend Vec3 cross(const Vec3& v1, const Vec3& v2);
	friend float dot(const Vec3& v1, const Vec3& v2);
	friend float tripleProduct(const Vec3& v1, const Vec3& v2, const Vec3& v3);
};

inline Vec3::Vec3(float p0, float p1, float p2)
{
	e[0] = p0;
	e[1] = p1;
	e[2] = p2;
}

inline const Vec3& Vec3::operator+() const { return *this; }

inline Vec3 Vec3::operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

inline float Vec3::length() const
{
	return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
}

inline float Vec3::squaredLength() const
{
	return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
}

inline void Vec3::makeUnitVector()
{
	*this = *this / (*this).length();
}

inline float Vec3::minComponent() const
{
	float min = e[0];
	if(e[1] < min) min = e[1];
	if(e[2] < min) min = e[2];

	return min;
}

inline float Vec3::maxComponent() const
{
	float max = e[0];
	if(e[1] > max) max = e[1];
	if(e[2] > max) max = e[2];

	return max;
}

inline float Vec3::maxAbsComponent() const
{
	float max = fabs(e[0]);
	if(fabs(e[1]) > max) max = fabs(e[1]);
	if(fabs(e[2]) > max) max = fabs(e[2]);

	return max;
}

inline float Vec3::minAbsComponent() const
{
	float min = fabs(e[0]);
	if(fabs(e[1]) < min) min = fabs(e[1]);
	if(fabs(e[2]) < min) min = fabs(e[2]);

	return min;
}

inline int Vec3::indexOfMinComponent() const
{
	int index = 0;
	float min = e[0];
	if(e[1] < min) { min = e[1]; index = 1; }
	if(e[2] < min) { index = 2; }

	return index;
}

inline int Vec3::indexOfMinAbsComponent() const
{
	int index = 0;
	float min = fabs(e[0]);
	if(fabs(e[1]) < min) { min = fabs(e[1]); index = 1; }
	if(fabs(e[2]) < min) { index = 2; }

	return index;
}

inline bool operator==(const Vec3& v1, const Vec3& v2)
{
	if(v1.e[0] != v2.e[0]) return false;
	if(v1.e[1] != v2.e[1]) return false;
	if(v1.e[2] != v2.e[2]) return false;

	return true;
}

inline bool operator!=(const Vec3& v1, const Vec3& v2)
{
	return !(v1 == v2);
}

inline int Vec3::indexOfMaxComponent() const
{
	int index = 0;
	float max = e[0];
	if(e[1] > max) { max = e[1]; index = 1; }
	if(e[2] > max) { index = 2; }

	return index;
}

inline int Vec3::indexOfMaxAbsComponent() const
{
	int index = 0;
	float max = fabs(e[0]);
	if(fabs(e[1]) > max) { max = fabs(e[1]); index = 1; }
	if(fabs(e[2]) > max) { index = 2; }

	return index;
}

inline Vec3 operator*(float s, const Vec3& v)
{
	return Vec3(v.e[0]*s, v.e[1]*s, v.e[2]*s);
}

inline Vec3 operator*(const Vec3& v, float s)
{
	return Vec3(v.e[0]*s, v.e[1]*s, v.e[2]*s);
}

inline Vec3 operator/(const Vec3& v, float s)
{
	return Vec3(v.e[0]/s, v.e[1]/s, v.e[2]/s);
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.e[0]+v2.e[0], v1.e[1]+v2.e[1], v1.e[2]+v2.e[2]);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.e[0]-v2.e[0], v1.e[1]-v2.e[1], v1.e[2]-v2.e[2]);
}

inline Vec3& Vec3::operator+=(const Vec3& rhs)
{
	*this = *this + rhs;
	return *this;
}

inline Vec3& Vec3::operator=(const Vec3& rhs)
{
	e[0] = rhs.e[0];
	e[1] = rhs.e[1];
	e[2] = rhs.e[2];

	return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& rhs)
{
	*this = *this - rhs;
	return *this;
}

inline Vec3& Vec3::operator*=(float s)
{
	*this = *this * s;
	return *this;
}

inline Vec3& Vec3::operator/=(float s)
{
	*this = *this / s;
	return *this;
}

inline float dot(const Vec3& v1, const Vec3& v2)
{
	return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2)
{
	Vec3 v;
	
	v.e[0] = v1.y() * v2.z() - v1.z() * v2.y();
	v.e[0] = v1.z() * v2.x() - v1.x() * v2.z();
	v.e[0] = v1.x() * v2.y() - v1.y() * v2.x();

	return v;
}

inline Vec3 unitVector(const Vec3& v)
{
	float length = v.length();
	return v / length;
}

inline Vec3 minVec(const Vec3& v1, const Vec3& v2)
{
	Vec3 min(v1);

	if(v2.x() < v1.x()) min.setX(v2.x());
	if(v2.y() < v1.y()) min.setY(v2.y());
	if(v2.z() < v1.z()) min.setZ(v2.z());

	return min;
}

inline Vec3 maxVec(const Vec3& v1, const Vec3& v2)
{
	Vec3 max(v1);

	if(v2.x() > v1.x()) max.setX(v2.x());
	if(v2.y() > v1.y()) max.setY(v2.y());
	if(v2.z() > v1.z()) max.setZ(v2.z());

	return max;
}

#endif
