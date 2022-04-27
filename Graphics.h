#ifndef GRAPHICS_H
#define GRAPHICS_H

class Point;
class Polygon;

class Graphics {
public:
  static Point project(const Point& p, int r);
  static Polygon project(const Polygon& p, int r);
  static Point old_project(const Point& p, int windowWidth, int windowHeight);
  static Polygon old_project(const Polygon& p, int windowWidth, int windowHeight);
};

#endif
