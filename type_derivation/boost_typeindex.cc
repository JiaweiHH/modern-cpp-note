#include <iostream>
#include <vector>
#include <boost/type_index.hpp>

struct Widget {
  int a;
  int b;
};

template<typename T>
void f(const T param) {
  using boost::typeindex::type_id_with_cvr;
  std::cout << "T: \n" << type_id_with_cvr<T>().pretty_name() << "\n";
  std::cout << "param: \n" << type_id_with_cvr<decltype(param)>().pretty_name() << "\n";
  std::cout << "typeid T: \n" << typeid(T).name() << "\n";
  std::cout << "typeid param: \n" << typeid(param).name() << "\n";
}

int main() {
  std::vector<Widget> vec;
  Widget w;
  vec.push_back(w);
  const auto vi = vec;
  f(&vi[0]);
}