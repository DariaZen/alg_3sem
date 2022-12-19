#include <iostream>

struct Point {
  int64_t x;
  int64_t y;
  Point(int64_t x, int64_t y) : x(x), y(y) {}

  Point& operator-=(const Point& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Point operator-(const Point& other) const {
    Point copy = *this;
    copy -= other;
    return copy;
  }

  int64_t operator*(const Point& second) const {
    return (x * second.y - y * second.x);
  }

  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Point& other) const { return !(*this == other); }
};

struct Segment {
  Point first_end;
  Point second_end;
  Segment(int64_t x0, int64_t y0, int64_t x1, int64_t y1)
      : first_end(Point(x0, y0)), second_end(Point(x1, y1)) {}

  Point FindLeft() const {
    return first_end.x <= second_end.x ? first_end : second_end;
  }

  Point FindRight() const {
    return first_end.x > second_end.x ? first_end : second_end;
  }

  Point FindDown() const {
    return first_end.y <= second_end.y ? first_end : second_end;
  }

  Point FindUpper() const {
    return first_end.y > second_end.y ? first_end : second_end;
  }

  enum EndsPosition {
    kInDifferentPlanes,
    kOnOneLine,
    kInOnePlane,
  };

  static int FindSign(int64_t number) {
    switch (number) {
      case 0:
        return 0;
      default:
        return number > 0 ? 1 : -1;
    }
  }

  EndsPosition AreEndsInDifferentPlanes(const Segment& other) const {
    int64_t vector_product1 =
        (second_end - first_end) * (other.second_end - first_end);
    int64_t vector_product2 =
        (second_end - first_end) * (other.first_end - first_end);
    switch (FindSign(vector_product1 * vector_product2)) {
      case -1:
        return kInDifferentPlanes;
      case 0:
        return kOnOneLine;
      default:
        return kInOnePlane;
    }
  }
};

bool IsFirstSegmentLeft(const Segment& first, const Segment& second) {
  Point left = first.FindLeft();
  return left.x <= second.first_end.x && left.x <= second.second_end.x;
}

bool IsFirstSegmentDown(const Segment& first, const Segment& second) {
  Point down = first.FindDown();
  return down.y <= second.first_end.y && down.y <= second.second_end.y;
}

bool IsXIntersection(const Segment& first, const Segment& second) {
  if (IsFirstSegmentLeft(first, second)) {
    Point right = first.FindRight();
    Point left = second.FindLeft();
    return (right.x >= left.x);
  }
  Point right = second.FindRight();
  Point left = first.FindLeft();
  return (right.x >= left.x);
}

bool IsYIntersection(const Segment& first, const Segment& second) {
  if (IsFirstSegmentDown(first, second)) {
    Point upper = first.FindUpper();
    Point down = second.FindDown();
    return (upper.y >= down.y);
  }
  Point upper = second.FindUpper();
  Point down = first.FindDown();
  return (upper.y >= down.y);
}

bool IsIntersection(const Segment& first, const Segment& second) {
  using EndsPosition = Segment::EndsPosition;
  if (first.first_end == first.second_end &&
      second.first_end == second.second_end &&
      first.first_end != second.first_end) {
    return false;
  }
  EndsPosition are_in_different_planes1 =
      first.AreEndsInDifferentPlanes(second);
  EndsPosition are_in_different_planes2 =
      second.AreEndsInDifferentPlanes(first);
  if (are_in_different_planes1 == Segment::kInOnePlane ||
      are_in_different_planes2 == Segment::kInOnePlane) {
    return false;
  }
  switch (are_in_different_planes1) {
    case Segment::kInDifferentPlanes:
      return are_in_different_planes2 != Segment::kInOnePlane;
    default:
      return are_in_different_planes2 == Segment::kInDifferentPlanes
                 ? true
                 : IsXIntersection(first, second) &&
                       IsYIntersection(first, second);
  }
}

Segment ReadSegment() {
  int64_t x0;
  int64_t y0;
  int64_t x1;
  int64_t y1;
  std::cin >> x0 >> y0 >> x1 >> y1;
  Segment segment = Segment(x0, y0, x1, y1);
  return segment;
}

int main() {
  Segment first = ReadSegment();
  Segment second = ReadSegment();

  IsIntersection(first, second) ? std::cout << "YES" : std::cout << "NO";
}
