## 对右值使用 move，对左值使用 forward

- 在最后一次使用右值引用的时候调用 `std::move`，在最后一次使用通用引用的时候调用 `std::forward`
- 对按值返回的函数返回值，无论返回值是右值引用还是通用引用，都按照上一条规则执行
- 当返回值是局部变量的时候，不要使用 `std::move` 和 `std::forward`

如果希望通过利用对象右值性的方式传递给其他使用对象的函数，应该将此类对象的参数转换为右值。这不仅是 `std::move` 所做的，也是创建 `std::move` 的理由

```c++
class Widget {
public:
  Widget(Widget&& rhs) : name(std::move(rhs.name)), p(std::move(rhs.p)) {}
private:
  std::string name;
  std::shared_ptr<SomeDataStructure> p;
};
```

通用引用可能会绑定到右值对象上，通用引用只有使用右值初始化时，才将其强制转化为右值

```c++
class Widget {
public:
  template<typename T>
  void setName(T&& newName) {
    name = std::forward<T>(newName);
  }
};
```

上述两个例子总结下来就是：当传递给函数时，右值引用应该无条件转化为右值，通用引用应该有条件转化为右值

另外不应该把通用引用的函数拆成两个重载函数，一个参数绑定到左值，另一个绑定到右值。下面这个例子就是把上面的 `setName` 函数改为两个重载函数

> 这一点后面关于解决通用引用和重载组合产生的问题方法中会再提到，总之这里这部很推荐这样做，理由可以往下看

```c++
class Widget {
public:
  void setName(const std::string& newName) { name = newName; }
  void setName(std::string&& newName) { name = std::move(newName); }
};

w.setName("Adela Novak");
```

这样做会带来几个问题：

1. 编写和维护的代码更多，可扩展性差
2. 效率下降

对于第二个问题。如果使用通用引用版本，字符串字面值 `Adela Novak` 可以背传递给 `setName`，然后再转手传递给 `w` 内部的 `std::string` 赋值运算符，不会涉及中间对象的创建

但是如果使用重载版本，一次 `setName` 调用会包括以下内容，其开销比直接通过 `const char*` 赋值给 `std::string` 昂贵很多

- `std::string` 中间对象的构造函数调用
- `std::string` 赋值运算符调用
- `std::string` 析构函数调用

对于第一个问题。如果 `Widget::setName` 接受 `n` 个参数，由于每个参数既可以是左值也可以是右值，就需要重载 `2^n` 个函数。还有些函数接受无限个参数，对于这类函数只能使用通用引用，例如 `std::make_unique`、`std::make_shared`，并且这类函数必定会使用 `std::forward`

另外如果使用按值返回的函数，并且**返回值绑定到右值引用或者通用引用上**，需要对返回的引用使用 `std::move` 或者 `std::forward`

```c++
Matrix operator+(Matrix&& lhs, const Matrix& rhs) {
  lhs += rhs;
  // 在返回语句中将 lhs 转换为右值，lhs 可以移动到返回值的内存位置
  // 如果省略了 move，则会导致一次额外的拷贝
  return std::move(lhs);
}
```

> 如果 `Matrix` 不支持移动操作，酱紫转换为右值不会变差，因为右值可以直接被 `Matrix` 的拷贝构造函数使用。此时如果 `Matrix` 以后支持了移动操作，那么在下一次编译的时候就会受益。所以将 `std::move` 引用到返回语句中，不会损失什么，还可能获得收益

通用引用和 `std::forward` 的情况 `std::move` 类似

最后再来看下面这个例子，涉及到了编译器对于临时变量返回值的优化：

```c++
// 版本一
Widget makeWidget() {
  Widget w;
  return w;
}
// 版本二，进行优化
Widget makeWidget() {`n
  Widget w;
  return std::move(w);
}
```

这里对局部变量使用了 `std::move` 进行优化，这是有问题的

原因在于：`C++` 编译器在进行优化的时候，对于第一个版本可以在分配给函数返回值的内存中构造局部变量 `w`，从而避免赋值局部变量的需要。这就是所谓的返回值优化 `RVO`。`RVO` 有几个条件：

1. 局部变量与返回值的类型相同
2. 局部变量就是返回值

所以移动版本的 `Widget` 不会被 `RVO` 消除移动操作开销，因为只有返回值为局部对象的时候才进行 `RVO`，在这里返回值已经变成局部对象的引用了

另外如果满足 `RVO` 的条件，但是编译器选择不执行复制忽略，则编译器会自动将 `std::move` 隐式执行。这意味着，如果对按值返回局部对象的函数使用 `std::move`，这并不能帮助编译器，反而会阻碍编译器优化选项