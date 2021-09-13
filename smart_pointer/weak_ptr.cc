#include <memory>
#include <iostream>

struct A;
struct B;

struct A {
  std::shared_ptr<B> pointer;
  ~A() {
    std::cout << "A 被销毁\n";
  }
};

struct B {
  std::shared_ptr<A> pointer;
  ~B() {
    std::cout << "B 被销毁\n";
  }
};

int main() {
  /**
   * @brief 这里 a, b 都不会被销毁
   * 互相持有对方的指针，两个指针的引用计数都是 2
   * 离开作用域的时候，a 和 b 的引用计数只能 -1，这样由于引用计数不是 0 就无法被销毁
   */
  auto a = std::make_shared<A>();
  auto b = std::make_shared<B>();
  a->pointer = b;
  b->pointer = a;
  std::cout << "a.use_count(): " << a.use_count() << "\n";
  std::cout << "b.use_count(): " << b.use_count() << "\n\n";

  /**
   * @brief std::weak_ptr 是一种弱引用不会引起引用计数的增加
   * 如果将上述 struct A 和 struct B 中的 shared_ptr 换成 weak_ptr 就可以避免上述问题了
   * 
   * weak_ptr 没有 * 和 -> 操作符，不能对资源进行操作
   * weak_ptr 可以获得一个 shared_ptr，wp.lock() 函数
   * wp.expire() 检查资源是否已经被释放
   */
  
  std::weak_ptr<int> wp;
  {
    std::shared_ptr<int> sp = std::make_shared<int>(10);
    wp = sp;
    std::cout << "in scope: \n";
    std::cout << "sp.use_count(): " << sp.use_count() << "\n";
    std::cout << "wp.use_count(): " << wp.use_count() << "\n";
  }
  std::cout << "out scope: \n";
  std::cout << "wp.use_count(): " << wp.use_count() << "\n";
  std::cout << std::boolalpha << wp.expired() << "\n";
}