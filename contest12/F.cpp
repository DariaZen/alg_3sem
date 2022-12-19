#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

struct Point {
  int64_t x;
  int64_t y;
  Point() = default;
  Point(int64_t x, int64_t y) : x(x), y(y) {}

  bool operator<(const Point& other) const {
    if (x == other.x) {
      return y < other.y;
    }
    return x < other.x;
  }
};

bool ClockwiseRotation(const Point& first, const Point& second,
                       const Point& third) {
  return first.x * (second.y - third.y) + second.x * (third.y - first.y) +
             third.x * (first.y - second.y) <
         0;
}

bool CounterClockwiseRotation(const Point& first, const Point& second,
                              const Point& third) {
  return first.x * (second.y - third.y) + second.x * (third.y - first.y) +
             third.x * (first.y - second.y) >
         0;
}

struct ConvexHull {
  vector<Point> set;
  Point left;
  Point right;
  vector<Point> convex_hull;

  ConvexHull(const vector<Point>& recieved_set)
      : set(SortSet(recieved_set)),
        left(set[0]),
        right(set[set.size() - 1]),
        convex_hull(MakeConvexHull()) {}

  static vector<Point> SortSet(const vector<Point>& recieved_set) {
    vector<Point> sorted_set = recieved_set;
    std::sort(sorted_set.begin(), sorted_set.end());
    return sorted_set;
  }

  void ProcessPart(vector<Point>& part, size_t current_point,
                   bool rotation(const Point&, const Point&,
                                 const Point&)) const {
    const size_t kMinEnvelopSize = 2;
    if (current_point == set.size() - 1 ||
        rotation(left, set[current_point], right)) {
      while (part.size() >= kMinEnvelopSize &&
             !rotation(part[part.size() - kMinEnvelopSize],
                       part[part.size() - 1], set[current_point])) {
        part.pop_back();
      }
      part.push_back(set[current_point]);
    }
  }

  vector<Point> MakeConvexHull() const {
    vector<Point> upper_part{left};
    vector<Point> down_part{left};
    for (size_t p = 1; p < set.size(); ++p) {
      ProcessPart(upper_part, p, ClockwiseRotation);
      ProcessPart(down_part, p, CounterClockwiseRotation);
    }
    for (size_t p = down_part.size() - 2; p > 0; --p) {
      upper_part.push_back(down_part[p]);
    }
    return upper_part;
  }

  void Write() const {
    std::cout << convex_hull.size() << "\n";
    for (const auto& point : convex_hull) {
      std::cout << point.x << " " << point.y << "\n";
    }
  }
};

vector<Point> ReadSet() {
  int64_t points_number;
  std::cin >> points_number;
  vector<Point> set(points_number);
  for (int64_t i = 0; i < points_number; ++i) {
    int64_t x;
    int64_t y;
    std::cin >> x >> y;
    set[i] = Point(x, y);
  }
  return set;
}

int main() {
  vector<Point> set = ReadSet();
  ConvexHull convex_hull(set);
  convex_hull.Write();
}
