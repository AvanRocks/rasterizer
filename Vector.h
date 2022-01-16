#ifndef VECTOR_H
#define VECTOR_H

#include "Point.h"

class Vector : public Point {
public:
	double x, y, z;
  Vector();
  Vector(double x, double y, double z);
  Vector& operator+=(const Vector& rhs);
  Vector& operator*=(const double& scalar);
  Vector& operator/=(const double& scalar);
};

Vector operator+(Vector lhs, const Vector& rhs);
Vector operator*(Vector lhs, const double& scalar);
Vector operator/(Vector lhs, const double& scalar);

#endif
