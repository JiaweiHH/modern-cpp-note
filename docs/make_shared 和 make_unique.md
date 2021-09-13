## make_shared 和 make_unique

- 与直接使用 `new` 相比，`make` 函数消除了重复代码，提高了异常安全性。对于 `make_shared` 和 `allocate_shared`，生成的代码更小更快
- 不适合使用 `make` 函数的情况包括需要自定义析构器和希望使用大括号初始化
- 对于 `shared_ptr` 和它的 `make` 函数，可能不被建议的情况还包括
  1. 有自定义内存管理的类
  2. 特别关注内存的系统，非常大的对象，以及 `weak_ptr` 比对应的 `shared_ptr` 生命周期更长的场景

`make functions` 有三个：`make_unique`, `make_shared`, `allocated_shared`。相比于使用 `new` 来构造智能指针，使用 `make function` 有更多的优点

**使用 `make function` 可以实现异常安全**，假设我们又如下两个函数，并且使用 `new` 的结果作为实参来构造 `shared_ptr`

```c++
// 按照某种优先级来处理 Widget
void processWidget(std::shared<Widget> spw, int priority);
// 优先级计算函数
int computePriority();
processWidget(std::shared_ptr<Widget>(new Widget), computePriority());
```

此时执行要 `processWidget()`，必须先执行以下操作：

- 表达式 `new Widget` 执行，在堆上创建一个对象

- 负责管理 `new` 出来的指针的 `shared_ptr<Widget>` 构造函数被执行

- `computePriority()` 执行

但是编译器会进行重拍，也就是说上面三部分内容最后的执行步骤可能是下面这样：

1. 执行 `new Widget`
2. 执行 `computePriority()`
3. 运行 `shared_ptr` 的构造函数

> 注：如果在 shared_ptr 的构造函数抛出了异常，那么不管是自定义析构器还是默认析构器都会在 new 出来的指针上调用。例如 `std::shared_ptr<Widget> spw(new Widget, cusDel);`

此时如果在运行 `computePriority()` 的时候产生了异常，那么第一步动态分配的 `Widget` 对象就会泄漏。而如果使用 `make_shared` 就不会出现这样的异常

另外**使用 `make_shared` 允许编译器产生更小、更快的代码，并且使用更加简洁的数据结构**。下面这段代码会进行内存分配，但是实际上它进行了两次内存分配，一次是为 `Widget` 分配内存，另一次是为该对象的控制块分配内存。而使用 `make_shared` 一次分配内存就足够了，这是因为 `make_shared` 一次分配的内存同时容纳了 `Widget` 对象和控制块

```c++
std::shared_ptr<Widget> spw(new Widget);
```

`make function` 也有不能做到的情况：

1. `make` 函数不能自定义析构器

2. `make` 函数不能转发大括号初始化列表，如果想要使用初始化列表构造函数必须自己先定义一个 `std::initializer_list` 对象

```c++
// 下面两个指针指向的 vector 都是使用小括号 (10, 20) 构造出来的
auto upv = std::make_unique<std::vector<int>>(10, 20);
auto spv = std::make_shared<std::vector<int>>(10, 20);

auto initList = {10, 20};
// 此时的 vector 是使用大括号 {10, 20} 构造出来的
auto spv = std::make_shared<std::vector<int>>(initList);
```

另外 `make_shared` 和 `shared_ptr` 对比 `make_unique` 还有至少两个问题。首先是如果 `Widget` 类自定义了 `operator new` 和 `operator delete`，并且这两个运算符是精确分配、释放对象的，也就是说只处理 `sizeof(Widget)` 大小的内存块的分配和释放。此时就不太适合使用 `shared_ptr`，因为 `std::allocate_shared` 需要的内存总大小不等于动态分配的对象大小，还需要额外加上控制块的大小

第二个问题源自于 `make_shared` 相较于 `new` 的大小和速度上的优势：`make_shared` 只会分配一次内存，会把控制块与指向的对象放在同一块内存中。所以只有当控制块被销毁的时候，该内存才会被销毁

而控制块除了对象引用计数还有第二个引用计数 `weak_count`，负责会记录有多少个 `weak_ptr` 指向控制块。当一个 `weak_ptr` 检测对象是否过期的时候，它会检测指向控制块中的对象引用计数，如果对象引用计数为 0 则说明过期了

因此只要 `weak_ptr` 引用一个控制块，则该控制块就必须存在，而只要控制块存在那么包含它的内存就必须保持分配。所以使用 `make_shared` 分配的内存，直到最后一个 `shared_ptr` 和最后一个指向它的 `weak_ptr` 被销毁，才会被释放

```c++
class ReallyBigType {/*...*/};	// 一个占用大内存的类

auto pBigObj = std::make_shared<ReallyBigType>();
// - 创建 std::shared_ptrs 和 std::weak_ptrs 指向这个对象，并使用它们
// - 最后一个 std::shared_ptr 在这里被销毁，但是 std::weak_ptr 还存在
// - 在这个阶段，原来分配给大对象的内存还分配者
// - 最后一个 std::weak_ptr 在这里被销毁，控制块和对象的内存被释放

std::shared_ptr<ReallyBigType> pBigObj1(new ReallyBigType);
// - 创建 std::shared_ptrs 和 std::weak_ptrs 指向这个对象，并使用它们
// - 最后一个 std::shared_ptr 在这里被销毁，但是 std::weak_ptr 还存在
// - memory for object is deallocated
// - 最后一个 std::weak_ptr 在这里被销毁，控制块内存被释放
```

