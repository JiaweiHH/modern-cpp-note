#include <iostream>

int foo(int a, int b, int c) {
  return a + b + c;
}

int main() {
  /* 将参数 1 和 2 绑定到函数 foo 上，并对 第一个参数进行占位 */
  auto bind_foo = std::bind(foo, std::placeholders::_1, 1, 2);

  /* 调用的时候只需要提供第一个参数即可 */
  std::cout << bind_foo(1) << "\n";
}