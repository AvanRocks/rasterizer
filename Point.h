#ifndef POINT_H
#define POINT_H

class Vector;

class Point {
public:
	int x, y, z;
	Point();
	Point(int x, int y, int z);
	Point& operator+=(const Vector &vector);
	Point& operator-=(const Vector &vector);
	static Point getMidpoint(Point p, Point q);
};

bool operator==(const Point& lhs, const Point &rhs);
bool operator!=(const Point& lhs, const Point &rhs);
Point operator+(Point point, const Vector &vector);
Point operator-(Point point, const Vector &vector);

#endif
