#ifndef _RGB_H_
#define _RGB_H_

#include <iostream>

class rgb
{
	public:

	// Member variables
	float _r, _g, _b;

	// Constructors
	rgb(float r=0, float g=0, float b=0);
	rgb(const rgb& original) { *this = original; }

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

	// I/O Operator
	friend std::ostream& operator<<(std::ostream& out, const rgb& c);

	// Clamp to (0, 1.0) range
	void clamp();
};

// Constructor
inline rgb::rgb(float r, float g, float b) : _r(r), _g(g), _b(b) {}

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

#endif
