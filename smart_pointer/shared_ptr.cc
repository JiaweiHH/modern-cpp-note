#include <memory>
#include <iostream>

void foo(std::shared_ptr<int> i) {
  (*i)++;
}

int main() {
  auto p = std::make_shared<int>(10); /* constructed */
  foo(p);
  std::cout << *p << "\n";

  auto pointer = std::make_shared<int>(10);
  auto pointer2 = pointer;  // 引用计数 +1
  auto pointer3 = pointer;  // 引用计数 +1
  int *p_1 = pointer.get(); // 获取原始指针，不会增加引用计数
  std::cout << "pointer.use_count() = " << pointer.use_count() << "\n";
  std::cout << "pointer2.use_count() = " << pointer2.use_count() << "\n";
  std::cout << "pointer3.use_count() = " << pointer3.use_count() << "\n";

  pointer2.reset(); // 取消 pointer2 的引用
  std::cout << "reset pointer2: \n";
  std::cout << "pointer.use_count() = " << pointer.use_count() << "\n";
  std::cout << "pointer2.use_count() = " << pointer2.use_count() << "\n";
  std::cout << "pointer3.use_count() = " << pointer3.use_count() << "\n";

  pointer3.reset(); // 取消 pointer3 的引用
  std::cout << "pointer.use_count() = " << pointer.use_count() << "\n";
  std::cout << "pointer2.use_count() = " << pointer2.use_count() << "\n";
  std::cout << "pointer3.use_count() = " << pointer3.use_count() << "\n";
}