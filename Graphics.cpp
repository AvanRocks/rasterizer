#include "Graphics.h"

#include "Point.h"
#include "Polygon.h"

#include <vector>

using namespace std;

Point Graphics::project(const Point& p, int r) {
  const double SCALING_FACTOR = (double) r / ( r - p.z );
  return Point( p.x * SCALING_FACTOR, p.y * SCALING_FACTOR, 0);
}

Polygon Graphics::project(const Polygon& polygon, int r) {
	vector<Point> newVertices;
	for (Point p : polygon.vertices) {
		newVertices.push_back(Graphics::project(p, r));
	}

	return Polygon(newVertices);
}

// OLD method of projecting
Point Graphics::old_project(const Point& p, int windowWidth, int windowHeight) {
  const double X_SCALING_FACTOR = (double) windowWidth / (windowWidth + 2 * p.z);
  const double Y_SCALING_FACTOR = (double) windowWidth / (windowHeight + 2 * p.z);
  return Point( p.x * X_SCALING_FACTOR, p.y * Y_SCALING_FACTOR, 0);
}

// OLD method of projecting
Polygon Graphics::old_project(const Polygon& polygon, int windowWidth, int windowHeight) {
	vector<Point> newVertices;
	for (Point p : polygon.vertices) {
		newVertices.push_back(Graphics::old_project(p, windowWidth, windowHeight));
	}

	return Polygon(newVertices);
}
