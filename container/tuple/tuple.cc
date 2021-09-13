#include <tuple>
#include <iostream>
auto get_student(int id) {
  /* 返回类型被推断为 std::tuple<double, char, std::string> */
  if(id == 0)
    return std::make_tuple(3.8, 'A', "张三");
  if(id == 1)
    return std::make_tuple(2.9, 'C', "李四");
  if(id == 2)
    return std::make_tuple(1.7, 'D', "王五");
  return std::make_tuple(0.0, 'D', "null");
}

int main() {
  auto student = get_student(0);
  std::cout << "ID: 0, "
            << "GPA: " << std::get<0>(student)
            << ", 成绩: " << std::get<1>(student)
            << ", 姓名: " << std::get<2>(student) << "\n";

  double gpa;
  char grade;
  std::string name;

  /* std::tie 对元组进行拆包 */
  std::tie(gpa, grade, name) = get_student(1);
    std::cout << "ID: 0, "
            << "GPA: " << gpa
            << ", 成绩: " << grade
            << ", 姓名: " << name << "\n";

  /* C++14，std::get 可以使用类型来获取元组中的对象 */
  std::tuple<std::string, double, double, int> t("123", 4.5, 6.7, 8);
  std::cout << std::get<std::string>(t) << "\n";
  // std::cout << std::get<double>(t) << "\n";  // 非法
  std::cout << std::get<3>(t) << "\n";
}