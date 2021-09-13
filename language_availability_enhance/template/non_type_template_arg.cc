#include <iostream>

/* 非类型模版参数 */
template<typename T, int BufSize>
class buffer_t {
public:
  T &alloc();
  void free(T &item);
private:
  T data[BufSize];
};
buffer_t<int, 100> buf;

/* C++17 使用 auto 推导非类型模版参数 */
template<auto value> void foo() {
  std::cout << value << "\n";
}

int main() {
  foo<100>();
}