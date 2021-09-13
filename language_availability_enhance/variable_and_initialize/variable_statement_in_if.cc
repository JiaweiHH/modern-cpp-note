#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  std::vector<int> vec = {1, 2, 3, 4};

  /* C++17 之前 */
  const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 2);
  if(itr != vec.end())
    *itr = 3;
  // 需要重新定一个变量
  const std::vector<int>::iterator itr2 = std::find(vec.begin(), vec.end(), 3);
  if(itr2 != vec.end())
    *itr2 = 4;
  
  /* C++17 之后，临时变量可以在 if/switch 语句里面声明 */
  if(const auto itr = std::find(vec.begin(), vec.end(), 4); itr != vec.end())
    *itr = 4;
}