#include "ONB.h"

#define ONB_EPSILON 0.01f

void ONB::initFromU(const Vec3& u)
{
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0f, 1.0f, 0.0f);

	U = unitVector(u);
	V = cross(U, n);
	if(V.length() < ONB_EPSILON)
		V = cross(U, m);
	W = cross(U, V);
}

void ONB::initFromV(const Vec3& v)
{
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0f, 1.0f, 0.0f);

	V = unitVector(v);
	U = cross(V, n);
	if(U.length() < ONB_EPSILON)
		U = cross(V, m);
	W = cross(U, V);
}

void ONB::initFromW(const Vec3& w)
{
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0f, 1.0f, 0.0f);

	W = unitVector(w);
	U = cross(W, n);
	if(U.length() < ONB_EPSILON)
		U = cross(W, m);
	V = cross(W, U);
}

void ONB::initFromUV(const Vec3& u, const Vec3& v)
{
	U = unitVector(u);
	W = unitVector(cross(u, v));
	V = cross(W, U);
}

void ONB::initFromVU(const Vec3& v, const Vec3& u)
{
	V = unitVector(v);
	W = unitVector(cross(u, v));
	U = cross(V, W);
}

void ONB::initFromUW(const Vec3& u, const Vec3& w)
{
	U = unitVector(u);
	V = unitVector(cross(w, u));
	W = cross(U, V);
}

void ONB::initFromWU(const Vec3& w, const Vec3& u)
{
	W = unitVector(w);
	V = unitVector(cross(w, u));
	U = cross(V, W);
}

void ONB::initFromVW(const Vec3& v, const Vec3& w)
{
	V = unitVector(v);
	U = unitVector(cross(v, w));
	W = cross(U, V);
}

void ONB::initFromWV(const Vec3& w, const Vec3& v)
{
	W = unitVector(w);
	U = unitVector(cross(v, w));
	V = cross(W, U);
}

bool operator==(const ONB& b1, const ONB& b2)
{
	return (b1.u() == b2.u() && b1.v() == b2.v() && b1.w() == b2.w());
}

std::istream& operator>>(std::istream& is, ONB& basis)
{
	Vec3 new_u, new_v, new_w;
	is >> new_u >> new_v >> new_w;
	basis.initFromUV(new_u, new_v);

	return is;	
}

std::ostream& operator<<(std::ostream& os, const ONB& basis)
{
	os << basis.u() << "\n" << basis.v() << "\n" << basis.w() << "\n";
	return os;
}
