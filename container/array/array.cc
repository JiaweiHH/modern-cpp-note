#include <array>
#include <iostream>
#include <vector>
#include <algorithm>

/**
 * @brief vector 容器的内存需要手动释放
 * vector 不会自动归还被删除元素的内存
 */
void vector_memmory_management() {
  std::vector<int> v;
  std::cout << "size: " << v.size() << "\n";
  std::cout << "capacity: " << v.capacity() << "\n";

  /* vector 的存储是自动管理的，按需自动扩张 */
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  std::cout << "size: " << v.size() << "\n";
  std::cout << "capacity: " << v.capacity() << "\n";

  v.push_back(4);
  v.push_back(5);
  std::cout << "size: " << v.size() << "\n";
  std::cout << "capacity: " << v.capacity() << "\n";

  /* 虽然清空了元素，但是被清空元素的内存并没有归还 */
  v.clear();
  std::cout << "size: " << v.size() << "\n";
  std::cout << "capacity: " << v.capacity() << "\n";

  /* 额外的内存可以通过 shrink_to_fit() 调用返回给系统 */
  v.shrink_to_fit();
  std::cout << "size: " << v.size() << "\n";
  std::cout << "capacity: " << v.capacity() << "\n";
}

void array() {
  constexpr int len = 4;  // array 大小必须是常量表达式
  std::array<int, len> arr = {1, 2, 3, 4};
  std::cout << "is array empty: " << std::boolalpha << arr.empty() << "\n";
  std::cout << "array size: " << arr.size() << "\n";

  /* 标准库算法支持 */
  std::sort(arr.begin(), arr.end(), [](int a, int b) {
    return b < a;
  });

  /* 迭代器支持 */
  for(auto &i : arr)
    std::cout << i << " ";
  std::cout << "\n";

  // int *arr_p = arr;  // std::array 不会自动退化成 T*
  /* C 风格接口传参 */
  int *p1 = &arr[0];
  int *p2 = arr.data();
}