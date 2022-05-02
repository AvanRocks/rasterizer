#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Vector.h"
#include "Point.h"

#include <vector>

class Polygon;

class Graphics {
private:
  // Velocity vectors in pixels per second
  static Vector X_VEL, Y_VEL, Z_VEL;

  int FPS;

  // z-coordinate of the center of projection
  static Point R;

  std::vector<Polygon> polygons;
  std::vector<Polygon> rotatedPolygons;

  static Point project(const Point& p);
  static Polygon project(const Polygon& p);
  static bool isInView(const Point& p);
  static bool isInView(const Polygon& poly);
  static Point intersectXY(const Point& p, const Point& q);
  static void clipToView(std::vector<Polygon>& polygons);

public:
  Graphics(std::vector<Polygon> polygons, int FPS);

  void addPolygons(std::vector<Polygon> newPolygons);

  void moveLeft(double angleX);
  void moveRight(double angleX);
  void moveForward(double angleX, double angleY);
  void moveBackward(double angleX, double angleY);
  void moveUp();
  void moveDown();

  void turn(double angleX, double angleY);

  void render(std::vector<Polygon>& screenPolygons);
};

#endif
