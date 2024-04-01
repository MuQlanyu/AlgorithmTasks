#include <iomanip>
#include <iostream>
#include "geometry.h"
using namespace Geometry;

Circle ReadCircle() {
  long double center;
  long double radius;
  std::cin >> radius >> center;
  return {{0, center}, radius};
}

std::pair<Point, Point> Intersection(const Circle& circle, const Line& line) {
  Vector vec(line.GetDirection()[0], line.GetDirection()[1]);
  Point cent = circle.Center();
  long double rad = circle.Radius();
  long double sz = std::sqrt(vec.VectorSize());
  vec.x /= sz;
  vec.y /= sz;
  return {cent + vec * rad, cent - vec * rad};
}

std::pair<Line, Line> Tangent(const Circle& circle, const Point& point) {
  long double rad = circle.Radius();
  long double len = Distance(point, circle.Center());
  len = std::sqrt(len * len - rad * rad);
  long double degree = len / rad;

  return {Line(point, degree), Line(point, -degree)};
}

std::pair<Line, Line> Tangent(const Circle& left, const Circle& right) {
  // left - выше, right - ниже
  if (left.Radius() == right.Radius()) {
    Line first(left.Center() + Point(left.Radius(), 0), right.Center() + Point(right.Radius(), 0));
    Line second(left.Center() + Point(-left.Radius(), 0), right.Center() + Point(-right.Radius(), 0));
    return {first, second};
  }
  long double sz = Distance(left.Center(), right.Center());
  long double ratio;
  if (left.Radius() < right.Radius()) {
    ratio = left.Radius() / right.Radius();
  } else {
    ratio = right.Radius() / left.Radius();
  }
  ratio = 1 - ratio;
  long double add = sz / ratio - sz;
  Point point;
  if (left.Radius() < right.Radius()) {
    point = left.Center() + Point(0, add);
    return Tangent(left, point);
  }
  point = right.Center() + Point(0, -add);
  return Tangent(right, point);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cout << std::fixed << std::setprecision(Math::kPrecision);

  Circle first = ReadCircle();
  Circle second = ReadCircle();

  if (first.Radius() == second.Radius()) {
    std::cout << first.Radius();
    return 0;
  }

  std::pair<Line, Line> lines = Tangent(second, first);
  Point inter = lines.first.Intersection(lines.second);
  if (inter.y >= 0 && inter.y <= first.Center().y) {
    std::cout << 0;
  } else {
    Line main_line = Line(Point(0, 0), Point(1, 0));
    inter = lines.first.Intersection(main_line);
    std::cout << Distance(Point(0, 0), inter);
  }
}
