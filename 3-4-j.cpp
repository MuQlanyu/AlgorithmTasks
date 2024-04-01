#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

namespace Math {
const int kPrecision = 6;
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
}  // namespace Geometry

using namespace Geometry;

std::vector<Point> ReadPoly() {
  size_t num;
  std::cin >> num;
  std::vector<Point> ans(num);
  for (size_t i = 0; i < num; ++i) {
    std::cin >> ans[i].x >> ans[i].y;
  }
  return ans;
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

  static std::pair<bool, bool> ProcessSimple(const PVector& minc,
                                             const Point& point) {
    size_t sz = minc.size();
    if (point.x < minc[0].x || (point.x == minc[0].x && point.y < minc[0].y)) {
      return {true, false};
    }
    if (Angle(minc[0], point) == Angle(minc[0], minc[sz - 1])) {
      return {true,
              Distance(minc[0], point) <= Distance(minc[0], minc[sz - 1])};
    }
    if (Angle(minc[0], point) == Angle(minc[0], minc[1])) {
      return {true, Distance(minc[0], point) <= Distance(minc[0], minc[1])};
    }
    if (Angle(minc[0], minc[sz - 1]) < Angle(minc[0], point) ||
        Angle(minc[0], point) < Angle(minc[0], minc[1])) {
      return {true, false};
    }
    return {false, false};
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

  static bool Check(const PVector& minc, const Point& point) {
    std::pair<bool, bool> simple = ProcessSimple(minc, point);
    if (simple.first) {
      return simple.second;
    }

    Angle point_angle(minc[0], point);

    size_t right = minc.size() - 1;
    size_t left = 1;
    size_t mid;
    Angle angle;
    while (right - left > 1) {
      mid = (left + right) / 2;
      angle = Angle(minc[0], minc[mid]);
      if (angle < point_angle) {
        left = mid;
      } else {
        right = mid;
      }
    }
    return DotProduct(minc[0], point, minc[right]) >= 0 &&
           DotProduct(minc[right], point, minc[left]) >= 0 &&
           DotProduct(minc[left], point, minc[0]) >= 0;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cout << std::fixed << std::setprecision(Math::kPrecision);
  std::vector<Point> poly1 = ReadPoly();
  std::vector<Point> poly2 = ReadPoly();
  std::vector<Point> poly3 = ReadPoly();

  std::vector<Point> minc = Minc::FindMinc(poly1, poly2);
  minc = Minc::FindMinc(minc, poly3);

  Point point;
  size_t num;
  std::cin >> num;
  for (size_t i = 0; i < num; ++i) {
    std::cin >> point.x >> point.y;
    std::cout << (Minc::Check(minc, point * 3) ? "YES" : "NO") << '\n';
  }
}
