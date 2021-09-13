#include <iostream>
#include <vector>
#include <string>

class A {
public:
  int *pointer;

  A() : pointer(new int(1)) {
    std::cout << "构造\n";
  }
  A(A &a) : pointer(new int(*a.pointer)) {
    std::cout << "拷贝\n";
  }
  A(A &&a) : pointer(a.pointer) {
    a.pointer = nullptr;
    std::cout << "移动\n";
  }
  ~A() {
    std::cout << "析构\n";
    delete pointer;
  }
};

/* 返回一个将亡值 */
A return_value(bool test) {
  A a, b;
  if(test)
    return a; /* 等价于 static_cast<A&&>(a) */
  return b;   /* 等价于 static_cast<A&&>(b) */
}

void use_standard_lib() {
  std::string str = "Hello World.";
  std::vector<std::string> v;

  /* 使用 push_back(const T&)，产生拷贝行为 */
  v.push_back(str);
  std::cout << "str: " << str << "\n";

  /* push_back(const T&&)，不会出现拷贝行为 */
  v.push_back(std::move(str));
  std::cout << "str: " << str << "\n";  // 会输出空的 str
}

int main() {
  /**
   * 1. return_value 首先会在内部构造两个 A 的对象
   * 2. 函数返回后，产生一个将亡值，被 A 的移动构造函数引用，延长了生命周期
   * 3. 取得这个右值中的指针保存在 obj，而将亡值的指针被设置为 nullptr，防止这块区域在析构的时候被销毁
   */
  A obj = return_value(false);
  std::cout << "obj: \n";
  std::cout << obj.pointer << "\n";
  std::cout << *obj.pointer << "\n\n";

  use_standard_lib();
}