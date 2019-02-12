#ifndef _ONB_H_
#define _ONB_H_

#include "vector3.h"

class ONB
{
	public:

	Vec3 U, V, W;

	ONB() {};
	ONB(const Vec3& a, const Vec3& b, const Vec3& c) { U = a; V = b; W = c; }

	void initFromU(const Vec3& u);
	void initFromV(const Vec3& v);
	void initFromW(const Vec3& w);

	void set(const Vec3& a, const Vec3& b, const Vec3& c) { U = a; V = b; W = c; }

	// Calculate the ONB from two vectors
	// The first one is the Fixed vector (it is just normalized)
	// The second is normalized and its direction can be adjusted
	void initFromUV(const Vec3& u, const Vec3& v);
	void initFromVU(const Vec3& v, const Vec3& u);

	void initFromUW(const Vec3& u, const Vec3& w);
	void initFromWU(const Vec3& w, const Vec3& u);

	void initFromVW(const Vec3& v, const Vec3& w);
	void initFromWV(const Vec3& w, const Vec3& v);

	friend std::istream& operator>>(std::istream& is, ONB& basis);
	friend std::ostream& operator<<(std::ostream& os, const ONB& basis);
	friend bool operator==(const ONB& b1, const ONB& b2);

	// Getters
	Vec3 u() const { return U; }
	Vec3 v() const { return V; }
	Vec3 w() const { return W; }
};

#endif
