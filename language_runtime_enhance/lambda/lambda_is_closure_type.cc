/**
 * lambda 表达式的本质是 一个和函数对象类型相似的类类型（称为闭包类型）的对象（称为闭包对象）
 * 当 lambda 表达式的捕获列表为空的时候，闭包对象能够转换为函数指针进行传递
 */

#include <iostream>

using foo = void(int);
void functional(foo f) {  /* 参数列表中的函数类型被视为退化后的函数指针类型 foo* */
  f(1);
}

int main() {
  /**
   * lambda 表达式本质上是闭包类型，当捕获列表为空的时候可以转化为函数指针进行传递
   */
  auto f = [](int value) {
    std::cout << value << "\n";
  };
  functional(f);  /* 传递闭包对象，隐式转换为 foo* 类型的指针 */
  f(1);
}