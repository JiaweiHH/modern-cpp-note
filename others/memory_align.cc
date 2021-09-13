#include <iostream>

/**
 * @brief C++11 提供了 alignof 和 alignas 来支持对内存对齐的控制
 * 1. alignof 获得一个与平台相关的 std::size_t 类型的值，用于查询该平台的对齐方式
 * 2. alignas 来重新修饰某个结构体的对齐方式
 */

struct Storage {
  char a;
  int b;
  double c;
  long long d;
};

/**
 * @brief max_align_t 要求每个变量类型的对齐方式严格一样，因此它几乎是最大对齐方式
 * 大部分平台上得到的结果都是 long double
 */
struct alignas(std::max_align_t) AlignasStorage {
  char a;
  int b;
  double c;
  long long d;
};

int main() {
  std::cout << alignof(Storage) << "\n";
  std::cout << alignof(AlignasStorage) << "\n";
}