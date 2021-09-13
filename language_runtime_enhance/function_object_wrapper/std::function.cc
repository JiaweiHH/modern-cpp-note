#include <iostream>

/**
 * @brief std::function
 * std::function 实例可以对任何可以调用的目标实体进行存储、复制和调用
 * std::function 就是函数的容器，将函数作为对象进行处理
 */

int foo(int para) {
  return para;
}

int main() {
  /* std::function 包装了一个返回值为 int，参数为 int 的函数 */
  std::function<int(int)> func = foo;

  /* 包装一个 lambda 表达式 */
  int important = 10;
  std::function<int(int)> func2 = [&](int value) -> int {
    return 1 + value + important;
  };

  std::cout << func(10) << "\n";
  std::cout << func2(10) << "\n";
}