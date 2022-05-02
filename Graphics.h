#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Vector.h"

#include <vector>

class Point;
class Polygon;

class Graphics {
private:
  // Velocity vectors in pixels per second
  Vector X_VEL, Y_VEL, Z_VEL;

  int FPS;

  // z-coordinate of the center of projection
  const int R = -800; 

  std::vector<Polygon> polygons;
  std::vector<Polygon> rotatedPolygons;

  static Point project(const Point& p, int r);
  static Polygon project(const Polygon& p, int r);
  static Point clip(const Point& inView, const Point& outOfView, int windowWidth, int windowHeight, int R);
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
