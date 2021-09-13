#include <iostream>

template<typename T>
auto print_type_info(const T &t) {
  /* C++17 支持在编译器就判断 if 的分支 */
  if constexpr(std::is_integral<T>::value)
    return t + 1;
  else {
    return t + 0.001;
  }
}

int main() {
  std::cout << print_type_info(5) << "\n";
  std::cout << print_type_info(3.14) << "\n";
}