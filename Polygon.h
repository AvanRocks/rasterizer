#ifndef POLYGON_H
#define POLYGON_H

#include "Point.h"

#include <vector>

class Vector;

class Polygon {
public:
	std::vector<Point> vertices;
	Polygon();
	Polygon(std::initializer_list<Point> list);
	Polygon(std::vector<Point>& vertices);
	Polygon& operator+=(const Vector &vector);
	Polygon& operator-=(const Vector &vector);
};

Polygon operator+(Polygon point, const Vector &vector);
Polygon operator-(Polygon point, const Vector &vector);

#endif
