#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

namespace Math {
const int kPrecision = 8;
const long double kMinute = 60;
}  // namespace Math

long double Ld(long long val) { return static_cast<long double>(val); }

namespace Geometry {
struct Point {
  Point(long long first, long long second) : x(first), y(second){};
  Point() = default;

  long long x = 0;
  long long y = 0;
};

using Vector = Point;

Point operator+(const Point& lhs, const Point& rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}

Point operator-(const Point& lhs, const Point& rhs) {
  return {lhs.x - rhs.x, lhs.y - rhs.y};
}

bool operator==(const Point& lhs, const Point& rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

Point operator*(const Point& point, long long val) {
  return {point.x * val, point.y * val};
}

long double DotProduct(const Vector& left, const Vector& right) {
  return left.x * right.y - right.x * left.y;
}

long double DotProduct(const Point& first, const Point& second,
                       const Point& third) {
  return DotProduct(second - first, third - second);
}

long long Distance(const Point& left, const Point& right) {
  return std::pow(left.x - right.x, 2) + std::pow(left.y - right.y, 2);
}

struct Angle {
  long long x;
  long long y;

  Angle() = default;
  Angle(const Point& lhs, const Point& rhs)
      : x(rhs.x - lhs.x), y(rhs.y - lhs.y) {}
};

bool operator==(const Angle& lhs, const Angle& rhs) {
  if (lhs.x == 0 && rhs.x == 0) {
    return lhs.y > 0 == rhs.y > 0;
  }
  if ((lhs.x > 0) != (rhs.x > 0)) {
    return false;
  }
  return lhs.y * rhs.x - lhs.x * rhs.y == 0;
}

bool operator<=(const Angle& lhs, const Angle& rhs) {
  if (lhs == rhs) {
    return true;
  }
  if (lhs.x == 0) {
    return lhs.y > 0 && rhs.x < 0;
  }
  if (rhs.x == 0) {
    return rhs.y < 0 || lhs.x > 0;
  }
  if (lhs.x > 0 != rhs.x > 0) {
    return rhs.x < 0;
  }
  return lhs.y * rhs.x < lhs.x * rhs.y;
}

bool operator<(const Angle& lhs, const Angle& rhs) {
  return lhs <= rhs && !(lhs == rhs);
}

class Line {
 private:
  using LPoint = std::pair<long double, long double>;
  LPoint start_;
  std::vector<long double> direction_;

 public:
  Line(const Point& first, const Point& second);
  Line(const Point& point, long double degree);
  Line() = default;

  Line Normal(const Point& start_point) const;
  LPoint Intersection(const Line& other) const;
  long double Dist(const Point& point) const;
};

Line::Line(const Point& first, const Point& second)
    : Line(first, Ld(second.y - first.y) / Ld(second.x - first.x)) {
  if (first.x == second.x) {
    // Вертикальная прямая
    direction_[0] = 0;
    direction_[1] = 1;
  }
}

Line::Line(const Point& point, long double degree)
    : start_({Ld(point.x), Ld(point.y)}), direction_{1, degree} {}

Line Line::Normal(const Point& start_point) const {
  if (direction_[1] == 0) {
    return {start_point, start_point + Point(0, 1)};
  }
  if (direction_[0] == 0) {
    return {start_point, start_point + Point(1, 0)};
  }
  return {start_point, -1 / direction_[1]};
}

std::pair<long double, long double> Line::Intersection(
    const Line& other) const {
  // Предполагается, что они пересекаются
  long double coefficient =
      (other.direction_[0] * (start_.second - other.start_.second) -
       other.direction_[1] * (start_.first - other.start_.first)) /
      (direction_[0] * other.direction_[1] -
       other.direction_[0] * direction_[1]);
  LPoint tmp = {direction_[0] * coefficient, direction_[1] * coefficient};
  tmp.first += start_.first;
  tmp.second += start_.second;
  return tmp;
}

long double Line::Dist(const Geometry::Point& point) const {
  Line normal = Normal(point);
  LPoint inter = Intersection(normal);
  LPoint l_point = {Ld(point.x) - inter.first, Ld(point.y) - inter.second};
  return std::sqrt(l_point.first * l_point.first +
                   l_point.second * l_point.second);
}
}  // namespace Geometry

using namespace Geometry;

std::vector<Point> ReadPoly(size_t num) {
  std::vector<Point> ans(num);
  for (size_t i = 0; i < num; ++i) {
    std::cin >> ans[i].x >> ans[i].y;
  }
  return ans;
}

class Minc {
  using PVector = std::vector<Point>;

  static size_t FindStart(const PVector& points) {
    Point start = points[0];
    size_t ind = 0;
    for (size_t i = 1; i < points.size(); ++i) {
      if (points[i].x < start.x) {
        start = points[i];
        ind = i;
      } else if (points[i].x == start.x && points[i].y < start.y) {
        start = points[i];
        ind = i;
      }
    }
    return ind;
  }

 public:
  static PVector FindMinc(const PVector& lhs, const PVector& rhs) {
    size_t lhs_start = FindStart(lhs);
    size_t rhs_start = FindStart(rhs);

    PVector ans(lhs.size() + rhs.size());
    size_t left = 0;
    size_t right = 0;
    Angle l_angle;
    Angle r_angle;

    size_t ind = 0;
    while (left < lhs.size() && right < rhs.size()) {
      ans[ind++] = lhs[(lhs_start + left) % lhs.size()] +
                   rhs[(rhs_start + right) % rhs.size()];
      l_angle = Angle(lhs[(lhs_start + left) % lhs.size()],
                      lhs[(lhs_start + left + 1) % lhs.size()]);
      r_angle = Angle(rhs[(rhs_start + right) % rhs.size()],
                      rhs[(rhs_start + right + 1) % rhs.size()]);
      if (l_angle == r_angle) {
        ++left;
        ++right;
      } else if (l_angle < r_angle) {
        ++left;
      } else {
        ++right;
      }
    }
    while (left < lhs.size() || right < rhs.size()) {
      ans[ind++] = lhs[(lhs_start + left) % lhs.size()] +
                   rhs[(rhs_start + right) % rhs.size()];
      if (left < lhs.size()) {
        ++left;
      } else {
        ++right;
      }
    }

    ans.resize(ind);
    return ans;
  }
};

void MAkeAero(std::vector<Point>& aero) {
  for (auto& point : aero) {
    point.x = -point.x;
    point.y = -point.y;
  }
}

void FindShortest(std::vector<Point>& minc) {
  Point point(0, 0);
  Line line(minc[0], minc[minc.size() - 1]);
  long double ans = line.Dist(point);
  for (size_t i = 1; i < minc.size(); ++i) {
    line = Line(minc[i], minc[i - 1]);
    ans = std::min(ans, line.Dist(point));
  }
  ans -= Math::kMinute;
  std::cout << std::max(Ld(0), ans);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cout << std::fixed << std::setprecision(Math::kPrecision);

  size_t sz1;
  size_t sz2;
  std::cin >> sz1 >> sz2;
  std::vector<Point> aero = ReadPoly(sz1);
  std::vector<Point> cloud = ReadPoly(sz2);
  MAkeAero(aero);

  std::vector<Point> minc = Minc::FindMinc(aero, cloud);
  FindShortest(minc);
}
