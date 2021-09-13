#include <iostream>

template<typename... T>
auto avg(T... args) {
  auto sum = (args + ...);
  auto num = sizeof...(args);
  return sum / num;
}

int main() {
  std::cout << avg(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << "\n";
}