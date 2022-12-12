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
    if (x == other.x && y == other.y) {
      return true;
    }
    return false;
  }

  bool operator!=(const Point& other) const { return !(*this == other); }
};

struct Segment {
  Point A;
  Point B;
  Segment(int64_t x0, int64_t y0, int64_t x1, int64_t y1)
      : A(Point(x0, y0)), B(Point(x1, y1)) {}

  Point find_left() const {
    if (A.x <= B.x) {
      return A;
    }
    return B;
  }

  Point find_right() const {
    if (A.x > B.x) {
      return A;
    }
    return B;
  }

  Point find_down() const {
    if (A.y <= B.y) {
      return A;
    }
    return B;
  }

  Point find_upper() const {
    if (A.y > B.y) {
      return A;
    }
    return B;
  }

  int64_t are_ends_in_different_planes(const Segment& other) const {
    int64_t vector_product1 = (B - A) * (other.B - A);
    int64_t vector_product2 = (B - A) * (other.A - A);
    if (vector_product1 * vector_product2 < 0) {
      return -1;
    }
    if (vector_product1 * vector_product2 == 0) {
      return 0;
    }
    return 1;
  }
};

bool is_first_segment_left(const Segment& AB, const Segment& CD) {
  Point left = AB.find_left();
  if (left.x <= CD.A.x && left.x <= CD.B.x) {
    return true;
  }
  return false;
}

bool is_first_segment_down(const Segment& AB, const Segment& CD) {
  Point down = AB.find_down();
  if (down.y <= CD.A.y && down.y <= CD.B.y) {
    return true;
  }
  return false;
}

bool is_x_intersection(const Segment& AB, const Segment& CD) {
  if (is_first_segment_left(AB, CD)) {
    Point right = AB.find_right();
    Point left = CD.find_left();
    return (right.x >= left.x);
  }
  Point right = CD.find_right();
  Point left = AB.find_left();
  return (right.x >= left.x);
}

bool is_y_intersection(const Segment& AB, const Segment& CD) {
  if (is_first_segment_down(AB, CD)) {
    Point upper = AB.find_upper();
    Point down = CD.find_down();
    return (upper.y >= down.y);
  }
  Point upper = CD.find_upper();
  Point down = AB.find_down();
  return (upper.y >= down.y);
}

bool is_intersection(const Segment& AB, const Segment& CD) {
  if (AB.A == AB.B && CD.A == CD.B && AB.A != CD.A) {
    return false;
  }
  int64_t are_in_different_planes1 = AB.are_ends_in_different_planes(CD);
  int64_t are_in_different_planes2 = CD.are_ends_in_different_planes(AB);
  if (are_in_different_planes1 == -1 && are_in_different_planes2 == -1) {
    return true;
  }
  if (are_in_different_planes1 == -1 && are_in_different_planes2 == 0) {
    return true;
  }
  if (are_in_different_planes1 == 0 && are_in_different_planes2 == -1) {
    return true;
  }

  if (are_in_different_planes1 == 0 && are_in_different_planes2 == 0) {
    if (is_x_intersection(AB, CD) && is_y_intersection(AB, CD)) {
      return true;
    }
  }
  return false;
}

Segment read_segment() {
  int64_t x0;
  int64_t y0;
  int64_t x1;
  int64_t y1;
  std::cin >> x0 >> y0 >> x1 >> y1;
  Segment segment = Segment(x0, y0, x1, y1);
  return segment;
}

int main() {
  Segment AB = read_segment();
  Segment CD = read_segment();

  if (is_intersection(AB, CD)) {
    std::cout << "YES";
    return 0;
  }
  std::cout << "NO";
}
