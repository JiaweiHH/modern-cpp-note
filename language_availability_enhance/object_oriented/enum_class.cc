#include <iostream>
#include <boost/type_index.hpp>

/**
 * @brief C++11 枚举类，未指定类型的时候默认为 int
 * 在 C++11 之前枚举类型会被视作整数，则会让两种完全不同的枚举类型可以直接比较
 * 并且在同一个命名空间中不同枚举类型的枚举值名字不能相同
 * 
 * C++11 枚举类不能够被隐式的转换为整数
 */
// valuei 的类型为 new_enum&&，其底层型别是 unsigned int
enum class new_enum : unsigned int {
  value1,
  value2,
  value3 = 100,
  value4 = 100
};

/**
 * @brief 重载针对 enum class 的 operator<<
 * std::enable_if<a, b> 如果 a 成立就在 std::enable_if<a, b> 中 typedef b 类型
 * 
 * @tparam T 
 * @param stream 
 * @param e 
 * @return std::ostream& 
 */
template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, T e) {
  return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

template<typename T>
void get_type(T&& param) {
  using boost::typeindex::type_id_with_cvr;
  std::cout << "param type: " << type_id_with_cvr<decltype(param)>().pretty_name() << "\n";
}

int main() {
  if(new_enum::value3 == new_enum::value4)
    std::cout << "new_enum::value3 == new_enum::value4\n";
  get_type(new_enum::value3);

  // unsigned int a = new_enum::value3; // 错误，不能直接获取枚举类型的值. 枚举值不会进行任何的隐式类型转换

  /* underlying_type 获取枚举值的类型，然后进行强制类型转换 */
  unsigned int a = static_cast<std::underlying_type<new_enum>::type>(new_enum::value3);
  std::cout << a << "\n";

  /* 使用重载的 << 操作符 */
  std::cout << new_enum::value4 << "\n";
}