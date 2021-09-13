#include <iostream>
#include <vector>
#include <initializer_list>

class Foo {
public:
  int value_a;
  int value_b;
  Foo(int a, int b) : value_a(a), value_b(b) {}
};

class MagicFoo {
public:
  std::vector<int> vec;
  MagicFoo(std::initializer_list<int> list) {
    for(std::initializer_list<int>::iterator it = list.begin(); it != list.end(); ++it)
      vec.push_back(*it);
  }

  /* 任意函数都可以使用初始化列表参数 */
  void foo(std::initializer_list<int> list) {
    // ...
  }
};

int main() {
  /* C++11 之前，类对象不能使用初始化列表初始化，数组和 POD 都可以 */
  int arr[3] = {1, 2, 3};
  Foo foo(1, 2);
  std::vector<int> vec = {1, 2, 3, 4, 5};

  /* C++11 之后类对象可以使用 初始化列表构造函数 */
  MagicFoo magicFoo = {1, 2, 3, 4, 5};  // 通过初始化列表构造函数进行隐式类型转换，然后进行拷贝构造

  /* 另外 C++11 还提供了统一的语法来初始化任意对象 */
  Foo foo2{3, 4};
}