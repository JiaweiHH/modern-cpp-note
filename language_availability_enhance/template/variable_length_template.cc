#include <iostream>

/* 递归调用变长参数模版 */
template<typename T>
void print1(T value) {
  std::cout << value << "\n";
}

template<typename T, typename... Ts>
void print1(T value, Ts... args) {
  std::cout << value << " ";
  print1(args...);
}

/* C++17 利用 constexpr if，不需要额外编写一个递归终止函数 */
template<typename T, typename... Ts>
void print2(T value, Ts... args) {
  std::cout << value << " ";
  /* TODO 这里必须要使用 if constexpr */
  if constexpr (sizeof...(args) > 0)
    print2(args...);
}

/* 初始化列表展开 */
template<typename T, typename... Ts>
auto print3(T value, Ts... args) {
  std::cout << value << " ";
  /**
   * @brief 会被展开为
   * { (lambda(args1), 0), (lambda(args2), 0), ... }
   * 这里的关键在于 ", 表达式"，它会先执行前面的内容然后把后面的内容作为赋值的变量
   */
  (void)std::initializer_list<T>{
      ([&args] { std::cout << args << " "; }(), 0)...};
}

/* TODO C++17 折叠表达式 */
template<typename... T>
auto sum(T... t) {
  return (t + ...);
}

int main() {
  print1(1, 2, "123", 1.1);
  print2(1, 2, "321", 1.2); std::cout << "\n";
  print3(1, 2, "111", 1.3); std::cout << "\n";
  std::cout << sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << "\n";
}