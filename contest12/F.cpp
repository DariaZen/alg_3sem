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

bool cw_rotation(const Point& A, const Point& B, const Point& C) {
  return A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y) < 0;
}

bool ccw_rotation(const Point& A, const Point& B, const Point& C) {
  return A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y) > 0;
}

vector<Point> make_convex_hull(vector<Point>& set) {
  size_t number = set.size();
  std::sort(set.begin(), set.end());
  Point left = set[0];
  Point right = set[number - 1];
  vector<Point> upper_part{left};
  vector<Point> down_part{left};
  for (int64_t p = 1; p < number; ++p) {
    if (p == number - 1 || cw_rotation(left, set[p], right)) {
      while (upper_part.size() >= 2 &&
             !cw_rotation(upper_part[upper_part.size() - 2],
                          upper_part[upper_part.size() - 1], set[p])) {
        upper_part.pop_back();
      }
      upper_part.push_back(set[p]);
    }
    if (p == number - 1 || ccw_rotation(left, set[p], right)) {
      while (down_part.size() >= 2 &&
             !ccw_rotation(down_part[down_part.size() - 2],
                           down_part[down_part.size() - 1], set[p])) {
        down_part.pop_back();
      }
      down_part.push_back(set[p]);
    }
  }
  for (size_t p = down_part.size() - 2; p > 0; --p) {
    upper_part.push_back(down_part[p]);
  }
  return upper_part;
}

vector<Point> read_set() {
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

void write_convex_hull(const vector<Point>& convex_hull) {
  std::cout << convex_hull.size() << "\n";
  for (auto point : convex_hull) {
    std::cout << point.x << " " << point.y << "\n";
  }
}

int main() {
  vector<Point> set = read_set();
  vector<Point> conex_hull = make_convex_hull(set);
  write_convex_hull(conex_hull);
}
