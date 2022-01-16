#include "Vector.h"

Vector::Vector() : Point() {}

Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}

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

Vector operator*(Vector lhs, const double& scalar) {
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
