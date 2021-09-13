#include <iostream>
#include <string>

void reference(std::string& str) {
  std::cout << " left value\n";
}

void reference(std::string&& str) {
  std::cout << " right value\n";
}

void increase(int &v) {
  ++v;
}

int main() {
  /* 左右值引用 */
  std::string lv1 = "string";         /* lv1 是一个左值 */
  // std::string &&r1 = lv1;          /* 非法，右值不能引用左值 */
  std::string &&rv1 = std::move(lv1); /* std::move 可以将左值转化为右值 */
  std::cout << rv1 << "\n";

  const std::string &lv2 = lv1 + lv1; /* 常量左值引用可以绑定到右值 */
  // lv2 += "test";                   /* 非法，常量引用无法修改 */
  std::cout << lv1 + lv2 << "\n";

  std::string &&rv2 = lv1 + lv2;  /* 合法，lv1 + lv2 是一个右值，右值引用可以延长临时对象的声明周期 */
  rv2 += "test";                  /* 合法，非常量引用可以修改临时变量 */
  std::cout << rv2 << "\n";

  reference(rv2);     /* 输出左值，rv2 本身是一个左值 */
  reference("test");  /* 输出右值 */

  /**
   * @brief 非常量引用不能绑定到右值
   * 1. 为什么非常量不允许绑定到右值
   * 2. 为什么常量引用可以绑定到右值
   */

  /**
   * @brief 关于第一个问题的解答：
   * 假设可以绑定，下面的代码就会存在逻辑错误
   * 
   * 这里由于 int& 不能绑定 double 参数，
   * 因此必须产生一个临时值保存 s，从让当 increase 修改了临时值时，s 本身并没有被修改
   * 
   * 所以是不能绑定的，下面的语法是错误的
   */
  double s = 1;
  // increase(s);

  /**
   * @brief 关于第二个问题的解答
   * 原因很简单，因为 Fortran 需要
   */
}