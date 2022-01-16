#include "Point.h"
#include "Vector.h"

Point::Point() : x(0), y(0), z(0) {}

Point::Point(int x, int y, int z) : x(x), y(y), z(z) {}

Point Point::getMidpoint(Point p, Point q) {
	return Point((p.x+q.x)/2, (p.y+q.y)/2, (p.z+q.z)/2);
}

bool operator==(const Point& lhs, const Point& rhs) {
	return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

bool operator!=(const Point& lhs, const Point& rhs) {
	return !operator==(lhs,rhs);
}

Point& Point::operator+=(const Vector& vector) {
	x += (int) vector.x;
	y += (int) vector.y;
	z += (int) vector.z;

	return *this;
}

Point operator+(Point point, const Vector& vector) {
	return point += vector;
}

Point& Point::operator-=(const Vector& vector) {
	x -= (int) vector.x;
	y -= (int) vector.y;
	z -= (int) vector.z;

	return *this;
}

Point operator-(Point point, const Vector& vector) {
	return point -= vector;
}
