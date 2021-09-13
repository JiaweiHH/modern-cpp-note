#include <iostream>
#define LEN 10

int len_foo() {
  int i = 2;
  return i;
}

constexpr int len_foo_constexpr() {
  return 5;
}

/* C++11 constexpr 函数不允许使用分支语句 */
constexpr int fibonacci(const int n) {
  return n == 1 || n == 2 ? 1 : fibonacci(n - 1) + fibonacci(n - 2);
}

/**
 * constexpr 修饰的表达式会在编译期成为常量表达式
 * 1. constexpr 函数可以递归
 * 2. C++14 开始 constexpr 函数可以在内部使用局部变量、循环和分支语句
 */

int main() {
  char arr_1[10];   // 合法
  char arr_2[LEN];  // 合法

  int len = 10;
  // char arr_3[len];  // 非法

  const int len_2 = len + 1;
  constexpr int len_2_constexpr = 1 + 2 + 3;
  // char arr_4[len_2];         // 非法，数组大小必须是一个编译期常量
  char arr_4[len_2_constexpr];  // 合法

  // char arr_5[len_foo() + 5];         // 非法
  char arr_6[len_foo_constexpr() + 1];  // 合法

  std::cout << fibonacci(10) << std::endl;
}