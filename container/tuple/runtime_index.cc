#include <tuple>
#include <variant>
#include <iostream>
#include <string>

/**
 * @brief 运行期索引实现函数
 * 
 * @tparam n 当前的索引序号
 * @tparam T 
 * @param tpl 
 * @param i 目标索引序号
 * @return constexpr std::variant<T...> 
 */
template<size_t n, typename... T>
constexpr std::variant<T...> _tuple_index(const std::tuple<T...>& tpl, size_t i) {
  if constexpr(n >= sizeof...(T))
    throw std::out_of_range("越界.");
  if(i == n)
    return std::variant<T...>{std::in_place_index<n>, std::get<n>(tpl)};  // 在 index = n 的位置上构造
  return _tuple_index<(n < sizeof...(T) - 1 ? n + 1 : 0)>(tpl, i);  // 下一个索引
}

template<typename... T>
constexpr std::variant<T...> tuple_index(const std::tuple<T...>& tpl, size_t i) {
  return _tuple_index<0>(tpl, i); // 从 0 开始查找
}

template<typename T0, typename... Ts>
std::ostream& operator<<(std::ostream& s, std::variant<T0, Ts...> const& v) {
  std::visit([&](auto&& x){ s << x; }, v);  // 输出 variant 上已经构造的元素
  return s;
}

int main() {
  std::tuple<int, int, double> t(1, 2, 1.1);

  int index = 1;
  // int second = std::get<index>(t); // 错误，std::get 依赖一个编译期常量
  
  /**
   * @brief C++17 提供了 std::variant，可以使用它实现运行期索引
   * std::variant 类似于 union
   */
  std::cout << "use runtime index for tuple: \n";
  std::cout << tuple_index(t, index) << "\n";

  /* std::visit 输出 std::variant 上已经构造的元素 */
  std::cout << "variant cout: \n";
  std::variant<std::string, int, double> v{0.0};
  std::visit([&](auto&& x){
    std::cout << x << "\n";
  }, v);
}