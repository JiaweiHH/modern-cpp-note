#include <tuple>
#include <iostream>
#include <variant>
#include <string>

/* 非常数索引 */
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
  /**
   * @brief 合并 tuple
   * 合并之后的 tuple 类型为 std::tuple<double, char, string, double, char, string>
   */
  std::tuple<double, char, std::string> t1{1.1, 'A', "张三"};
  std::tuple<double, char, std::string> t2{2.2, 'B', "李四"};
  auto new_tuple = std::tuple_cat(t1, t2);
  using new_tuple_tpye = decltype(new_tuple);

  /* 使用之前提到的运行期索引完成 tuple 的遍历 */
  for(int i = 0; i != std::tuple_size<new_tuple_tpye>::value; ++i)
    std::cout << tuple_index(new_tuple, i) << " ";
  std::cout << "\n";
}