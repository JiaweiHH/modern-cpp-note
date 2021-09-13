#include <map>
#include <string>
#include <iostream>

/* 使用一行代码实现这个函数完成 hash 计算 */
template<typename Key, typename Value, typename F>
void update(std::map<Key, Value>& m, F foo) {
  /* auto&& 采用引用折叠 */
  for(auto&& [key, value] : m) value = foo(key);
}

int main() {
  std::map<std::string, long long int> m {
    {"a", 1},
    {"b", 2},
    {"c", 3}
  };
  update(m, [](std::string key) {
    return std::hash<std::string>{}(key);
  });
  for(auto&& [key, value] : m)
    std::cout << key << ":" << value << "\n";
}