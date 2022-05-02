#include "Graphics.h"

#include "Point.h"
#include "Polygon.h"
#include "Matrix.h"

#include <vector>
#include <cstdlib> // for abs()
#include <cstdio>

using namespace std;

Graphics::Graphics(vector<Polygon> polygons, int FPS) : X_VEL(500,0,0), Y_VEL(0,500,0), Z_VEL(0,0,500) {
	this->polygons = polygons;
	this->FPS = FPS;
}

void Graphics::addPolygons(std::vector<Polygon> newPolygons) {
	for (Polygon p : newPolygons) {
		this->polygons.push_back(p);

		// Initialize rotatedPolygons
		this->rotatedPolygons.push_back(p);
	}
}

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

void Graphics::moveLeft(double angleX) {
  for (Polygon& p : polygons)
  	p += Matrix::rotateY(X_VEL/FPS, -angleX);
}

void Graphics::moveRight(double angleX) {
  for (Polygon& p : polygons)
  	p -= Matrix::rotateY(X_VEL/FPS, -angleX);
}

void Graphics::moveForward(double angleX, double angleY) {
  for (Polygon& p : polygons)
    p -= Matrix::rotateX(Matrix::rotateY(Z_VEL/FPS, -angleX), angleY);
}

void Graphics::moveBackward(double angleX, double angleY) {
  for (Polygon& p : polygons)
    p += Matrix::rotateX(Matrix::rotateY(Z_VEL/FPS, -angleX), angleY);
}

void Graphics::moveUp() {
  for (Polygon& p : polygons)
		// Here we add because the positive y-axis is downward
		p += Y_VEL/FPS;
}

void Graphics::moveDown() {
  for (Polygon& p : polygons)
		// Here we subtract because the positive y-axis is downward
		p -= Y_VEL/FPS;
}

void Graphics::turn(double angleX, double angleY) {
	// Here, Matrix.rotateY() rotates about the y-axis.
	// Hence, angleX is passed to rotateY() (left-right view rotation).
	// Similarly, Matrix.rotateX() rotates about the x-axis.
	// So angleY is passed to rotateX() (up-down view rotation)
	for (int i = 0; i < polygons.size(); ++i)
		rotatedPolygons[i] = Matrix::rotateX(Matrix::rotateY(polygons[i], angleX), angleY);
}

void Graphics::render(std::vector<Polygon>& screenPolygons) {
	screenPolygons = rotatedPolygons;

	clipToView(screenPolygons);

	// Project the polygons onto the x-y plane
	for (int i = 0; i < screenPolygons.size(); ++i)
		screenPolygons[i] = Graphics::project(screenPolygons[i], R);
}

// Clip points to be above the x-y plane (positive z-coordinate)
void Graphics::clipToView(vector<Polygon> &polygons) {
	for (int i = 0; i < polygons.size(); ++i) {
		Polygon currPoly = polygons[i];

		// If one or more of the vertices are below the x-y plane
		// clip the polygon to have a positive z-coordinate
		if (!Graphics::isInView(currPoly)) {

			// All vertices of newPoly will have positive z-coordinate
			Polygon newPoly;

			// Number of vertices of the current polygon
			int numVertices = currPoly.vertices.size();

			// Loop over the current polgon's vertices 
			for (int j = 0; j < numVertices; ++j) {
				Point currPoint = currPoly.vertices[j];
				Point nextPoint = currPoly.vertices[(j+1) % numVertices];

				bool currPointInView = Graphics::isInView(currPoint);
				bool nextPointInView = Graphics::isInView(nextPoint);

				if (currPointInView) {
					newPoly.vertices.push_back(currPoint);
				}

				// If the points lie across the x-y plane
				if (currPointInView ^ nextPointInView) {
					Point intersectionPoint = Graphics::intersectXY(currPoint, nextPoint);

					// Add the intersectionPoint (which is on the x-y plane) to the new polygon
					newPoly.vertices.push_back(intersectionPoint);
				}
			}

			// Replace current polygon with clipped polygon
			polygons[i] = newPoly;
		}
	}
}

Point Graphics::intersectXY(const Point& p, const Point& q) {
	double t = (double) p.z / (p.z - q.z);
	Vector pq(p,q);
	Point ret = p + t * pq;
	ret.z = 0;
	return ret;
}