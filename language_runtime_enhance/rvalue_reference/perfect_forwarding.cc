#include <iostream>

/**
 * @brief forward 实现方式
 * 
 * @tparam _Tp 模版参数. 当传递的参数是左值的时候 _Tp = &，传递参数是右值的时候 _Tp = &&
 * @param __t 函数参数
 * @return constexpr _Tp&& 返回值进行引用折叠
 */
template<typename _Tp>
constexpr _Tp&& lmitate_forward(typename std::remove_reference<_Tp>::type& __t) noexcept
{ return static_cast<_Tp&&>(__t); }

template<typename _Tp>
constexpr _Tp&& lmitate_forward(typename std::remove_reference<_Tp>::type&& __t) noexcept {
  static_assert(!std::is_lvalue_reference<_Tp>::value, "template argument _Tp is an lvaue reference type");
  return static_cast<_Tp&&>(__t);
}


void reference(int &v) {
  std::cout << "左值引用\n";
}

void reference(int &&v) {
  std::cout << "右值引用\n";
}

template<typename T>
void pass(T &&v) {
  std::cout << "普通传参：";
  reference(v); // 普通传参都输出左值
  std::cout << "std::move 传参：";
  reference(std::move(v));

  /* forward 和 static_cast 从现象上看是一样的 */
  std::cout << "std::forward 传参：";
  reference(std::forward<T>(v));
  std::cout << "static_cast<T&&> 传参：";
  reference(static_cast<T&&>(v));
}

int main() {
  /**
   * @brief 传递右值，T 被推导为 int，v 的类型为 int &&
   * 1. reference(v)，对于 v 本身来说它是一个左值，所以输出左值
   * 2. reference(move(v))，move(v) 得到一个右值，所以输出右值
   * 3. reference(forward<T>(v))，forward<int>(v) 返回 int&&，所以输出右值
   * 4. reference(static_cast<int&&>(v))，等价于 forward，所以输出右值
   */
  std::cout << "传递右值：\n";
  pass(1);

  /**
   * @brief 传递左值，T 被推导为 int&，所以 v 的类型为 int& && = int&
   * 1. v，输出左值
   * 2. move(v)，输出右值
   * 3. forward<int&> v 返回 int& && = int& 所以输出左值
   * 4. static_cast 与 forward 一个意思，所以输出左值
   */
  std::cout << "传递左值：\n";
  int v = 1;
  pass(v);
}