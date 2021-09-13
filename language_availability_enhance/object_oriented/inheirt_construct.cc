#include <iostream>

class Base {
public:
  int value1;
  int value2;
  Base() : value1(1) {}
  Base(int value) : Base() {value2 = value;}
};

class SubClass : public Base {
public:
  /**
   * @brief C++11 继承构造函数
   * 在 C++11 之前需要定义派生类构造函数给基类构造函数传递参数，完成基类的初始化
   * C++11 开始通过 using 来继承基类的构造函数，避免了繁琐的编码
   */
  using Base::Base;

  // 假设派生类指示添加了一个函数
  void display() {}
};

int main() {
  SubClass s(3);  /* 如果派生类没有定义自己的构造函数，也没有继承基类的构造函数就会报错 */
  std::cout << s.value1 << ", " << s.value2 << "\n";
}