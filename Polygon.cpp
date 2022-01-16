#include "Polygon.h"
#include "Point.h"

#include <vector>

using namespace std;

Polygon::Polygon() {}

Polygon::Polygon(initializer_list<Point> list) {
	for (Point p : list) {
		vertices.push_back(p);
	}
}

Polygon::Polygon(vector<Point>& vertices) {
	this->vertices = vertices;
}

Polygon& Polygon::operator+=(const Vector &vector) {
	for (int i = 0; i < vertices.size(); ++i)
		vertices[i] += vector;

	return *this;
}

Polygon operator+(Polygon poly, const Vector &vector) {
	return poly += vector;
}


Polygon& Polygon::operator-=(const Vector &vector) {
	for (int i = 0; i < vertices.size(); ++i)
		vertices[i] -= vector;

	return *this;
}

Polygon operator-(Polygon poly, const Vector &vector) {
	return poly -= vector;
}

