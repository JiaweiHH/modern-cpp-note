#include <memory>
#include <iostream>

/* C++14 引入 make_unique */
std::unique_ptr<int> pointer = std::make_unique<int>(10);
// std::unique_ptr<int> pointer2 = pointer; // 非法

struct Foo {
  Foo() { std::cout << "Foo:Foo\n"; }
  ~Foo() { std::cout << "Foo:~Foo\n"; }
  void foo() { std::cout << "Foo:foo\n"; }
};

void f(const Foo&) {
  std::cout << "f(const Foo&)\n";
}

int main() {
  /* 移动构造函数 */
  std::unique_ptr<Foo> p1(std::make_unique<Foo>());
  if(p1)  // p1 不空，输出
    p1->foo();
  
  {
    std::unique_ptr<Foo> p2 = (std::move(p1));
    f(*p2); // p2 不空，输出
    if(p2)  // p2 不空，输出
      p2->foo();
    if(p1)  // p2 为空，不输出
      p1->foo();
    p1 = std::move(p2);
    if(p2)  // p2 为空，不输出
      p2->foo();
    std::cout << "p2 被销毁\n";
  }

  if(p1)  // p1 不空，输出
    p1->foo();
  
  // 离开作用域，foo 的内存实例被销毁
}