#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

using base = std::complex<double>;
using std::vector;
const double PI = 3.14159265359;

void fft(vector<base>& polynom, bool is_inverted) {
  size_t size = polynom.size();
  if (size == 1) {
    return;
  }
  vector<base> even_part(size / 2);
  vector<base> odd_part(size / 2);
  for (size_t i = 0, j = 0; i < size; i += 2, ++j) {
    even_part[j] = polynom[i];
    odd_part[j] = polynom[i + 1];
  }
  fft(even_part, is_inverted);
  fft(odd_part, is_inverted);
  double angle = 2 * PI / size * (is_inverted ? -1 : 1);
  base current_root(1);
  base move_to_next_root(cos(angle), sin(angle));
  for (size_t i = 0; i < size / 2; ++i) {
    polynom[i] = even_part[i] + current_root * odd_part[i];
    polynom[i + size / 2] = even_part[i] - current_root * odd_part[i];
    if (is_inverted) {
      polynom[i] /= 2, polynom[i + size / 2] /= 2;
    }
    current_root *= move_to_next_root;
  }
}

vector<int> multiply(const vector<int>& polynom1, const vector<int>& polynom2) {
  vector<base> complex_polynom1(polynom1.begin(), polynom1.end());
  vector<base> complex_polynom2(polynom2.begin(), polynom2.end());
  size_t new_degree = 1;
  while (new_degree < std::max(polynom1.size(), polynom2.size())) {
    new_degree <<= 1;
  }
  new_degree <<= 1;
  complex_polynom1.resize(new_degree);
  complex_polynom2.resize(new_degree);
  fft(complex_polynom1, false);
  fft(complex_polynom2, false);
  for (size_t i = 0; i < new_degree; ++i) {
    complex_polynom1[i] *= complex_polynom2[i];
  }
  fft(complex_polynom1, true);
  vector<int> multipication_result(new_degree);
  for (size_t i = 0; i < new_degree; ++i) {
    multipication_result[i] = std::round(complex_polynom1[i].real());
  }
  return multipication_result;
}

vector<int> read_polynom() {
  int degree;
  std::cin >> degree;
  vector<int> polynom(degree + 1);
  for (int i = degree; i >= 0; --i) {
    std::cin >> polynom[i];
  }
  return polynom;
}

void write_multipication_result(vector<int>& multipication_result) {
  std::reverse(multipication_result.begin(), multipication_result.end());
  int degree = multipication_result.size() - 1;
  int leading_coefficient = 0;
  while (leading_coefficient < multipication_result.size() &&
         multipication_result[leading_coefficient] == 0) {
    --degree;
    ++leading_coefficient;
  }
  if (degree == -1) {
    std::cout << 0 << " " << 0;
    return;
  }
  std::cout << degree << " ";
  for (int i = leading_coefficient; i < multipication_result.size(); ++i) {
    std::cout << multipication_result[i] << " ";
  }
}

int main() {
  vector<int> polynom1 = read_polynom();
  vector<int> polynom2 = read_polynom();

  vector<int> multipication_result = multiply(polynom1, polynom2);
  write_multipication_result(multipication_result);
}