#include <iostream>

/**
 * @brief lambda 表达式的一般形式
 * [捕获列表](参数列表) mutable(可选) 异常属性 -> 返回类型 { // 函数体 }
 */


/**
 * @brief lambda 表达式值补货
 * 值捕获的前提是变量可以 copy，
 * 并且 copy 发生在 lambda 表达式创建的时候，而非调用时（函数发生在调用时）
 */
void lambda_value_capture() {
  int value = 1;
  auto copy_type = [value] {
    return value;
  };
  value = 100;
  auto stored_value = copy_type();
  std::cout << "stored_value = " << stored_value << "\n";
  // stored_value 的输出值是 1，在创建 copy_type 的时候就已经保存一份了
}

/**
 * @brief 引用捕获
 * 和函数的引用传参类似
 */
void lambda_reference_capture() {
  int value = 1;
  auto copy_value = [&value] {
    return value;
  };
  value = 100;
  auto stored_value = copy_value();
  std::cout << "stored_value = " << stored_value << "\n";
  // stored_value 的输出是 100，引用捕获 捕获的是引用 会随着外面的变化而改变
}

/**
 * @brief 隐式捕获
 * [] 空捕获列表
 * [name1, name2, ...] 捕获一系列变量
 * [&] 引用捕获，让编译器自行推导引用捕获列表
 * [=] 值捕获，让编译器自行推导值捕获列表
 */

/**
 * @brief 表达式捕获
 * C++14 允许捕获的成员用任意表达式进行初始化，这就允许了右值捕获
 * 被捕获的变量类型会自动判断，判断方式与 auto 本质上是一样的
 */
void lambda_expression_capture() {
  auto important = std::make_unique<int>(1);
  // important 是一个 unique_ptr 不能被 "=" 捕获，但是我们可以将它转化成右值
  auto add = [v1 = 1, v2 = std::move(important)](int x, int y) -> int {
    return x + y + v1 + (*v2);
  };
  std::cout << add(3, 4) << "\n";
}

/**
 * @brief 泛型 lambda
 * C++14 开始允许 lambda 表达式使用 auto 关键字
 * 普通函数不能使用 auto 参数，这会与模版冲突
 */
void lambda_auto_variable() {
  auto add = [](auto x, auto y) {
    return x + y;
  };
}

int main() {
  lambda_value_capture();
  lambda_reference_capture();
  lambda_expression_capture();
}