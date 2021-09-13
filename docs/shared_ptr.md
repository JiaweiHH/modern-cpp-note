## shared_ptr

- 相比于 `unique_ptr`，`shared_ptr` 对象通常大两倍，其中包含控制块
- 控制块会产生开销，需要使用原子引用计数修改，另外还有虚函数机制产生的开销（虚函数开销只承受一次，在对象析构的时候）
- 默认的资源销毁操作是 `delete`，但是也支持自定义析构器，析构器的类型不属于 `shared_ptr` 的型别
- 避免从原始指针上面创建 `shared_ptr`

通过 `std::shared_ptr` 这种智能指针访问的对象采用共享所有权来管理其生存期，`shared_ptr` 会确保在不再需要该对象的时刻将其析构。与 `unique_ptr` 一样，`shared_ptr` 也可以自定义析构器。但是区别在于对于 `unique_ptr` 来说析构器是其型别的一部分，而对于 `shared_ptr` 来说析构器不属于该型别

```c++
auto loggingDel = [](Widget* pw) {
  makeLogEntry(pw);
  delete pw;
};
// 析构器型别是智能指针型别的一部分
std::unique_ptr<Widget, decltype(loggingDel)> upw(new Widget, loggingDel);
// 析构器型别不是智能指针型别的一部分
std::shared_ptr<Widget> spw(new Widget, loggingDel);

auto customDel1 = [](Widget *pw) { /*...*/ };
auto customDel2 = [](Widget* pw) { /*...*/ };
std::shared_ptr<Widget> pw1(new Widget, customDel1);
std::shared_ptr<Widget> pw2(new Widget, customDel2);
// 两个具有不同析构器的 shared_ptr 可以放在一个容器内，因为析构器不属于智能指针型别的一部分
std::vector<std::shared<Widget>> vpw{pw1, pw2};
```

`shared_ptr` 的析构器不会改变智能指针本身的尺寸，这与 `unique_ptr` 不同，另外 `shared_ptr` 是用引用计数来管理共享资源的。在 `shared_ptr` 内部包含了两个指针，一个指针指向共享对象资源的指针，另一个指针指向一个控制块，因此其尺寸是裸指针的两倍。

一个对象的控制块由创建首个指涉到该对象的 `shared_ptr` 的函数来确定，控制块内部包含了引用计数、析构器的拷贝等数据，控制块的创建遵循以下规则：

1. `std::make_shared` 总是创建一个控制块
   - `make_shared` 会构造一个对象出来，因此在调用 `make_shared` 的时候显然不会有针对该对象的控制块存在
2. 从具备专属所有权的指针例如 `unique_ptr` 或 `auto_ptr` 出发构造一个 `shared_ptr` 时，会创建一个控制块
   - 专属所有权指针不会使用控制块
3. 当 `shared_ptr` 构造函数使用裸指针作为实参的时候，会创建一个控制块
4. 给一个 `shared_ptr` 构造函数传递 `shared_ptr` 或者 `weak_ptr` 不会创建控制块，因为它们可以依赖传入的智能指针以指涉到任意所需的控制块

规则三会导致一个后果：从裸指针出发创建多个 `shared_ptr` 会产生未定义的行为。这会导致同一块内存被释放多次

```c++
auto pw = new Widget;
std::shared_ptr<Widget> spw1(pw, loggingDel);	// 为 *pw 创建了第一个控制块
std::shared_ptr<Widget> spw2(pw, loggingDel);	// 为 *pw 创建了第二个控制块
```

> 一般而言需要避免直接传入裸指针，应该使用 `make_shared`。当时当采用自定义析构器的时候不能使用 `make_shared` 来创建，此时不得不依赖直接传入裸指针，这种情况下应该直接传递 `new` 运算符的结果

当使用 `shared_ptr` 来管理 `this` 指针的时候，可能会产生未定义的行为。在对象本身托管到一个 `shared_ptr` 的时候，下面这段代码会导致 `this` 对象的控制块被创建两次

```c++
std::vector<std::shared_ptr<Widget>> processWidgets;
class Widget {
public:
  void process() {
    processWidgets.emplace_back(this);
  }
};
```

针对上述问题标准库提供了 `std::enable_shared_from_this<>` 模版类，当希望一个托管到 `shared_ptr` 的类能够安全的由 `this` 指针创建一个 `shared_ptr` 的时候，只需要继承该类。`std::enable_shared_from_this<>` 定义了一个成员函数 `shared_from_this()`，它会创建一个 `shared_ptr` 指涉到当前对象，但同时不会重复创建控制块

`shared_from_this` 函数会查询当前对象的控制块，并创建一个指涉到该控制块的新的 `shared_ptr`。因此就必须有一个已经存在的指涉到当前对象的 `shared_ptr` 存在，否则该函数会抛出异常，为了避免这种异常出现可以让客户端通过工厂方法创建并且把构造函数声明为私有的

```c++
void Widget::process() {
  processWidgets.emplace_back(shared_from_this());
}
```

最后对于 `shared_ptr` 相比 `unique_ptr` 它是不能处理数组的，没有办法定义 `shared_ptr<T[]>`。原因在于

1. `shared_ptr` 没有提供 `[]` 重载
2. `shared_ptr` 支持转换为指向基类的指针，这只对于单个对象有效

