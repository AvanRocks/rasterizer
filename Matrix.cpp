#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Polygon.h"

#include <cmath>
#include <vector>

using namespace std;

Point Matrix::rotateX(const Point& p, double angle) {
	return Point(p.x, p.y * cos(angle) - p.z * sin(angle), p.y * sin(angle) + p.z * cos(angle));
}

Point Matrix::rotateY(const Point& p, double angle) {
	return Point(p.x * cos(angle) + p.z * sin(angle), p.y, -p.x * sin(angle) + p.z * cos(angle));
}

Vector Matrix::rotateX(const Vector& v, double angle) {
	return Vector(v.x, v.y * cos(angle) - v.z * sin(angle), v.y * sin(angle) + v.z * cos(angle));
}

Vector Matrix::rotateY(const Vector& v, double angle) {
	return Vector(v.x * cos(angle) + v.z * sin(angle), v.y, -v.x * sin(angle) + v.z * cos(angle));
}

Polygon Matrix::rotateX(const Polygon& poly, double angle) {
	vector<Point> newVertices;
	for (Point p : poly.vertices) {
		newVertices.push_back(Matrix::rotateX(p, angle));
	}

	return Polygon(newVertices);
}

Polygon Matrix::rotateY(const Polygon& poly, double angle) {
	vector<Point> newVertices;
	for (Point p : poly.vertices) {
		newVertices.push_back(Matrix::rotateY(p, angle));
	}

	return Polygon(newVertices);
}
