#include "Vector.h"

Vector::Vector() : Point() {}

Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}

Vector::Vector(const Point& p, const Point& q) : Vector(q.x - p.x, q.y - p.y, q.z - p.z) {}

Vector& Vector::operator+=(const Vector &rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

Vector operator+(Vector lhs, const Vector &rhs) {
	return lhs += rhs;
}

Vector& Vector::operator*=(const double& scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;

	return *this;
}

Vector operator*(const double& scalar, Vector lhs) {
	return lhs *= scalar;
}

Vector& Vector::operator/=(const double& scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;

	return *this;
}

Vector operator/(Vector lhs, const double& scalar) {
	return lhs /= scalar;
}
