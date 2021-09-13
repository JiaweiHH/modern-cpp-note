#include <atomic>
#include <thread>
#include <iostream>

/**
 * @brief C++11 提供了 atomic 模版，使得我们实例化一个原子类型
 * 对一个原子类型的读写操作从一组指令最小化到单个 CPU 指令
 * 
 * 对于浮点和整数原子类型还提供了成员函数，例如 fetch_add, fetch_sub
 * 以及重载了这两个函数的 +, - 版本
 * 
 * 并非所有的类型都提供原子操作，原子操作的可能性取决于 CPU 架构以及
 * 所实例化的类型是否满足该架构对内存对齐的要求
 * std::atomic<T>::is_lock_free 可以检查该原子类型是否支持原子操作
 */

struct A {
  float x;
  int y;
  long long z;
};

std::atomic<int> count = {10};

int main() {
  std::thread t1([] {
    count.fetch_add(1);
  });
  std::thread t2([] {
    count++;    // 等价于 fetch_add
    count += 1; // 等价于 fetch_add
  });

  t1.join();
  t2.join();
  std::cout << count << "\n";

  std::atomic<A> a;
  std::cout << std::boolalpha << a.is_lock_free() << "\n";
}