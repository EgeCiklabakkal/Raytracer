#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "vector3.h"

class Vertex
{
	public:

	Vec3 position;
	Vec3 normal;
	int incidentTriangleCount;

	Vertex() {}
	Vertex(const Vec3& _position) : 
	position(_position), normal(Vec3(0.0f, 0.0f, 0.0f)), incidentTriangleCount(0) {}
	Vertex(const Vec3& _position, const Vec3& _normal) : 
	position(_position), normal(_normal), incidentTriangleCount(0) {}

	void addToNormal(const Vec3& v);
	void averageNormal();
	void makeUnitNormal();
};

inline void Vertex::addToNormal(const Vec3& v)
{
	normal += v;
	incidentTriangleCount++;
}

inline void Vertex::averageNormal()
{
	normal /= incidentTriangleCount;
}

inline void Vertex::makeUnitNormal()
{
	normal.makeUnitVector();
}

#endif
