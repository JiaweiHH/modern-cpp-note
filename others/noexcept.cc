#include <iostream>

void may_throw() {
  throw true;
}

auto non_block_throw = [] {
  may_throw();
};

void no_throw() noexcept {
  return;
}

auto block_throw = []() noexcept {
  no_throw();
};

/**
 * @brief 使用 noexcept 修饰的函数抛出异常，
 * 编译器会使用 std::terminate() 来立即终止程序的运行
 * 
 * noexcept 还能够用作操作符来操作一个表达式，当表达式没有异常时返回 true，否则返回 false
 */

int main() {
  std::cout << std::boolalpha << "may_throw() noexcept? "
            << noexcept(may_throw()) << "\n"
            << "no_throw() noexcept()? " << noexcept(no_throw()) << "\n"
            << "lmay_throw() noexcept? " << noexcept(non_block_throw()) << "\n"
            << "lno_throw() noexcept()? " << noexcept(block_throw()) << "\n";
}