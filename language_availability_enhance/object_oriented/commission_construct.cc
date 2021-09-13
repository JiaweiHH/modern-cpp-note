#include <iostream>

class Base {
public:
  int value1;
  int value2;
  Base() : value1(1) {}

  /**
   * @brief C++11 委托构造函数
   * 委托构造函数不能使用成员初始化
   */
  Base(int value) : Base() { value2 = value; }
};

int main() {
  Base b(2);
  std::cout << b.value1 << ", " << b.value2 << "\n";
}