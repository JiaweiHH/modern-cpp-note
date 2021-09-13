## 引用饰词

- 成员函数引用饰词使得对于左值和右值对象 (`*this`) 的处理能够区分开来

```c++
class Widget {
public:
  using DataType = std::vector<double>;
  DataType& data() & { return values; }
  DataType data() && { return std::move(values); }  // 这里其实不需要 std::move 返回值本身就是一个右值了
private:
  DataType values;
};

// 工厂函数返回一个 Widget 右值对象
Widget makeWidget();

Widget w;
auto vals1 = w.data();            // w.data() 是一个左值，执行拷贝构造函数
auto vals2 = makeWidget().data(); // makeWidget().data() 是一个右值，执行移动构造函数
```

## override

虚函数的改写需要满足一系列的要求：

- 基类中的函数必须是虚函数（派生类中可以不需要 `virtual` 关键字）
- 基类和派生类中的函数名字必须完全相同（析构函数不必）
- 基类和派生类的函数形参必须完全相同
- 基类和派生类中函数的常量性必须完全相同
- 基类和派生类中函数返回值和异常规格必须兼容
- 基类和派生类中函数的引用饰词必须完全相同

`C++11` 提供了一种方法来显示地标明派生类中的函数是为了改写基类版本，从而可以让编译器来检查是不是满足了以上的要求：`override` 关键字

```c++
class Base {
public:
  virtual void mf1() const;
  virtual void mf2(int x);
  virtual void mf3() &;
  virtual void mf4() const;
};
class Derived : public Base {
public:
  virtual void mf1() const override;
  virtual void mf2(int x) override;
  virtual void mf3() & override;
  void mf4() const override;  // 加个 virtual 没问题，但也没必要
};
```

