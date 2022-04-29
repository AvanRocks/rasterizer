#include "Graphics.h"

#include "Point.h"
#include "Polygon.h"

#include <vector>
#include <cstdlib> // for abs()
#include <cstdio>

using namespace std;

Point Graphics::project(const Point& p, int r) {
	if (p.z < 0) {
    fprintf(stderr, "Error: Trying to project an invalid point.\n");
	}

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

// Find the point of intersection of line connecting the two points 
// inView and outOfView with the viewing frustum.
// This algorithm is NOT perfect. It can miss the intersection points 
// in some situations, when the binary search jumps over the viewing frustum.
// Good enough for now lol.
Point Graphics::clip(const Point& inView, const Point& outOfView, int windowWidth, int windowHeight, int R) {

	// Define mutable versions of inView and outOfView 
	// for use in the binary search
	Point inViewMut = inView;
	Point outOfViewMut = outOfView;

	Point midpoint;

	while (true) {
		midpoint = Point::getMidpoint(inViewMut, outOfViewMut);
		if (midpoint == inViewMut || midpoint == outOfViewMut)
			break;
		else if (Graphics::isInView(midpoint))
			inViewMut = midpoint;
		else
			outOfViewMut = midpoint;
	}

	// Return the point that is certainly in view
	return inViewMut;
}

bool Graphics::isInView(const Point& p) {
	return p.z >= 0;
}

// true if all four of the polygon's points are in view
bool Graphics::isInView(const Polygon& polygon) {
	for (int i = 0; i < polygon.vertices.size(); ++i) {
		if (!Graphics::isInView(polygon.vertices[i]));
			return false;
	}
	return true;
}