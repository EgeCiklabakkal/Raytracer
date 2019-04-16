#ifndef _RGB_H_
#define _RGB_H_

#include <iostream>
#include "vector3.h"

class rgb
{
	public:

	// Member variables
	float _r, _g, _b;

	// Constructors
	rgb();
	rgb(float value);
	rgb(float r, float g, float b);
	rgb(const rgb& original) { *this = original; }
	rgb(const Vec3& v);

	// Setters
	void setRed(float r)   { _r = r; }
	void setGreen(float g) { _g = g; }
	void setBlue(float b)  { _b = b; }

	// Getters
	float r() const { return _r; }
	float g() const { return _g; }
	float b() const { return _b; }

	// Assignment Operators
	rgb& operator=(const rgb& rhs);
	rgb& operator+=(const rgb& rhs);
	rgb& operator*=(const rgb& rhs);
	rgb& operator/=(const rgb& rhs);
	rgb& operator*=(float rhs);
	rgb& operator/=(float rhs);

	// Unary Operators
	rgb operator+() const { return *this; }
	rgb operator-() const { return rgb(-_r, -_g, -_b); }

	// Arithmetic Operators
	friend rgb operator*(const rgb& c, float s);
	friend rgb operator*(float s, const rgb& c);
	friend rgb operator/(const rgb& c, float s);
	friend rgb operator*(const rgb& c1, const rgb& c2);
	friend rgb operator/(const rgb& c1, const rgb& c2);
	friend rgb operator+(const rgb& c1, const rgb& c2);
	friend rgb operator-(const rgb& c1, const rgb& c2);

	// I/O Operator
	friend std::ostream& operator<<(std::ostream& out, const rgb& c);

	// Clamp to (0, 1.0) range
	void clamp();

	// Clamp to (0, 255) range
	void clamp256();

	//Methods
	void initFromBytes(const Vec3& color_in_bytes);
	Vec3 asVec3() const;
};

// Constructors
inline rgb::rgb() : _r(0), _g(0), _b(0) {}

inline rgb::rgb(float value) : _r(value), _g(value), _b(value) {}

inline rgb::rgb(float r, float g, float b) : _r(r), _g(g), _b(b) {}

inline rgb::rgb(const Vec3& v) : _r(v.e[0]), _g(v.e[1]), _b(v.e[2]) {}

// Assignment Operators
inline rgb& rgb::operator=(const rgb& rhs)
{
	this->_r = rhs._r;
	this->_g = rhs._g;
	this->_b = rhs._b;

	return *this;
}

inline rgb& rgb::operator+=(const rgb& rhs)
{
	*this = *this + rhs;
	
	return *this;
}

inline rgb& rgb::operator*=(const rgb& rhs)
{
	*this = *this * rhs;
	
	return *this;
}

inline rgb& rgb::operator/=(const rgb& rhs)
{
	*this = *this / rhs;
	
	return *this;
}

inline rgb& rgb::operator*=(float rhs)
{
	*this = *this * rhs;
	
	return *this;
}

inline rgb& rgb::operator/=(float rhs)
{
	*this = *this / rhs;
	
	return *this;
}

// Arithmetic Operators
inline rgb operator*(const rgb& c, float s)
{
	return rgb(c._r*s, c._g*s, c._b*s);
}

inline rgb operator*(float s, const rgb& c)
{
	return rgb(c._r*s, c._g*s, c._b*s);
}

inline rgb operator/(const rgb& c, float s)
{
	return rgb(c._r/s, c._g/s, c._b/s);
}

inline rgb operator*(const rgb& c1, const rgb& c2)
{
	return rgb(c1._r*c2._r, c1._g*c2._g, c1._b*c2._b);
}

inline rgb operator/(const rgb& c1, const rgb& c2)
{
	return rgb(c1._r/c2._r, c1._g/c2._g, c1._b/c2._b);
}

inline rgb operator+(const rgb& c1, const rgb& c2)
{
	return rgb(c1._r+c2._r, c1._g+c2._g, c1._b+c2._b);
}

inline rgb operator-(const rgb& c1, const rgb& c2)
{
	return rgb(c1._r-c2._r, c1._g-c2._g, c1._b-c2._b);
}

// I/O Operator
inline std::ostream& operator<<(std::ostream& out, const rgb& c)
{
	out << c._r << " " << c._g << " " << c._b << " ";

	return out;
}

// Clamp to (0, 1.0) range
inline void rgb::clamp()
{
	if(this->_r > 1.0f) this->_r = 1.0f;
	if(this->_g > 1.0f) this->_g = 1.0f;
	if(this->_b > 1.0f) this->_b = 1.0f;

	if(this->_r < 0.0f) this->_r = 0.0f;
	if(this->_g < 0.0f) this->_g = 0.0f;
	if(this->_b < 0.0f) this->_b = 0.0f;
}

// Clamp to (0, 255) range
inline void rgb::clamp256()
{
	if(this->_r > 255.0f) this->_r = 255.0f;
	if(this->_g > 255.0f) this->_g = 255.0f;
	if(this->_b > 255.0f) this->_b = 255.0f;

	if(this->_r < 0.0f) this->_r = 0.0f;
	if(this->_g < 0.0f) this->_g = 0.0f;
	if(this->_b < 0.0f) this->_b = 0.0f;
}

inline void rgb::initFromBytes(const Vec3& color_in_bytes)
{
	Vec3 fcolor = color_in_bytes / 256.0f;
	this->_r = fcolor.x();
	this->_g = fcolor.y();
	this->_b = fcolor.z();
	this->clamp();
}

inline Vec3 rgb::asVec3() const
{
	return Vec3(_r, _g, _b);
}

#endif
