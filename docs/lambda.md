## lambda

`Lambda` 的一般形式为 `[捕获列表](参数列表) mutable(可选) 异常属性 -> 返回类型 { // 函数体 }`

```c++
std::find_if(container.begin(), container.end(), [](int val){
  return 0 < val && val < 10;
});
```

- 闭包是 `lambda` 创建的运行时对象，在上面的 `std::find_if` 调用中闭包作为第三个实参在运行时传递给 `std::find_if`
- 闭包类是实例化闭包的类。每个 `lambda` 都会使编译器生成唯一的闭包类，`lambda` 中的语句成为闭包类的成员函数中的可执行指令

当 `lambda` 表达式的捕获列表为空的时候，闭包对象能够转换为函数指针进行传递

```c++
using foo = void(int);
void functional(foo f) {  /* 参数列表中的函数类型被视为退化后的函数指针类型 foo* */
  f(1);
}

auto f = [](int value) {
  std::cout << value << "\n";
};
functional(f);  /* 传递闭包对象，隐式转换为 foo* 类型的指针 */
f(1);
```

### 参数捕获

捕获只能应用于 `lambda` 被创建时作用域里的 `non-static` 局部变量；定义在全局空间或者命名空间，或者在类、函数、文件中声明为 `static` 的对象（这些对象拥有静态存储生命周期）不能被捕获。`lambda` 有以下几种捕获方式：值捕获、引用捕获、表达式捕获

- 在类成员函数中使用默认捕获方式，会捕获到类对象的 `this` 指针

#### 引用捕获

```c++
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
```

注意：当采用引用捕获的时候，如果创建的 `lambda` 生命周期大于被捕获的对象，则当对象析构的时候，`lambda` 中的引用会变成空悬引用

#### 按值捕获

```c++
/**
 * @brief lambda 表达式值捕获
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
```

#### 表达式捕获

```c++
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
```

