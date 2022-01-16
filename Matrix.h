#ifndef MATRIX_H
#define MATRIX_H

class Point;
class Vector;
class Polygon;

// 3 by 3 matrix only
class Matrix {
public:
	static Point rotateX(const Point& p, double angle); // angle in radians
	static Point rotateY(const Point& p, double angle); // angle in radians
	static Vector rotateX(const Vector& v, double angle); // angle in radians
	static Vector rotateY(const Vector& v, double angle); // angle in radians
	static Polygon rotateX(const Polygon& v, double angle); // angle in radians
	static Polygon rotateY(const Polygon& v, double angle); // angle in radians
};

#endif
