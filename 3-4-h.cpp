#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

namespace Math {
const long double kEpsilon = 0.0000000000000000001;  // 1e-6
}  // namespace Math

struct Point {
  long long x;
  long long y;

  Point() = default;
  Point(long long left, long long right) : x(left), y(right) {}

  long double Size() const {
    return std::sqrt(static_cast<long double>(x * x + y * y));
  }
};

using Vector = Point;

Point operator+(const Point& lhs, const Point& rhs) {
  return Point(lhs.x + rhs.x, lhs.y + rhs.y);
}

Point operator-(const Point& lhs, const Point& rhs) {
  return Point(lhs.x - rhs.x, lhs.y - rhs.y);
}

bool operator==(const Point& lhs, const Point& rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

long double ScalarProduct(const Vector& left, const Vector& right) {
  return left.x * right.x + left.y * right.y;
}

long long DotProduct(const Vector& left, const Vector& right) {
  return left.x * right.y - right.x * left.y;
}

long long DotProduct(const Point& first, const Point& second,
                     const Point& third) {
  return DotProduct(second - first, third - second);
}

long double Cos(const Point& first, const Point& second, const Point& third) {
  Vector first_segment = first - second;
  Vector second_segment = third - second;
  return ScalarProduct(first_segment, second_segment) / first_segment.Size() /
         second_segment.Size();
}

long long Distance(const Point& lhs, const Point& rhs) {
  long long first = lhs.x - rhs.x;
  long long second = lhs.y - rhs.y;
  return first * first + second * second;
}

class ConvexHull {
  std::vector<Point> points_;
  std::vector<Point> hull_;

  Point FindHullStart() {
    Point start = points_[0];
    for (auto& point : points_) {
      if (point.x < start.x) {
        start = point;
      } else if (point.x == start.x && point.y < start.y) {
        start = point;
      }
    }
    return start;
  }

  void MAkeAngleVector(std::vector<std::pair<long double, size_t>>& angle,
                       Point& start) {
    Point help_point = start + Point(0, -1);
    angle.resize(points_.size() - 1);

    size_t ind = 0;
    for (size_t i = 0; i < points_.size(); ++i) {
      if (points_[i] == start) {
        continue;
      }
      angle[ind++] = {Cos(help_point, start, points_[i]), i};
    }
    angle.resize(ind);
    auto cmp = [](auto& left, auto& right) { return left.first < right.first; };
    std::sort(angle.begin(), angle.end(), cmp);
  }

  void ProcessLast(std::vector<std::pair<long double, size_t>>& angle,
                   size_t last) {
    if (hull_.size() <= 1) {
      hull_.push_back(points_[angle[last].second]);
      return;
    }
    size_t cur_sz = hull_.size();
    long long tmp_angle = DotProduct(points_[angle[last].second], hull_.back(),
                                     hull_[cur_sz - 2]);
    while (cur_sz >= 1 && tmp_angle <= 0) {
      --cur_sz;
      tmp_angle = DotProduct(points_[angle[last].second], hull_[cur_sz - 1],
                             hull_[cur_sz - 2]);
    }
    hull_.resize(cur_sz);
    hull_.push_back(points_[angle[last].second]);
  }

  void MakeHull() {
    Point start = FindHullStart();
    std::vector<std::pair<long double, size_t>> angle;
    MAkeAngleVector(angle, start);

    hull_.push_back(start);
    if (angle.empty()) {
      return;
    }

    hull_.push_back(points_[angle[0].second]);
    size_t last = 0;
    for (size_t i = 1; i < angle.size(); ++i) {
      if (std::abs(angle[last].first - angle[i].first) < Math::kEpsilon) {
        if (Distance(start, points_[angle[last].second]) <
            Distance(start, points_[angle[i].second])) {
          last = i;
          hull_.pop_back();
          ProcessLast(angle, i);
        }
        continue;
      }
      ProcessLast(angle, i);
      last = i;
    }
  }

 public:
  ConvexHull(std::vector<Point>&& points) : points_(std::move(points)) {
    MakeHull();
  }

  void PrintHull() {
    std::cout << hull_.size() << '\n';
    for (auto& point : hull_) {
      std::cout << point.x << ' ' << point.y << '\n';
    }
  }

  long double FindArea() {
    long double area = DotProduct(hull_.back(), hull_[0]);
    for (size_t i = 1; i < hull_.size(); ++i) {
      area += static_cast<long double>(DotProduct(hull_[i - 1], hull_[i]));
    }
    return std::abs(area);
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cout << std::fixed << std::setprecision(0);

  size_t num;
  std::cin >> num;
  std::vector<Point> points(num);
  for (size_t i = 0; i < num; ++i) {
    std::cin >> points[i].x >> points[i].y;
  }

  ConvexHull hull(std::move(points));
  hull.PrintHull();
  std::cout << std::fixed << std::setprecision(1);
  std::cout << hull.FindArea() / 2;
}
