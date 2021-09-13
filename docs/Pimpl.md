## Pimpl

- `Pimpl` 习惯用法通过降低类的客户和类实现者之间的依赖性，减少了构建遍数
- 对于采用 `unique_ptr` 来实现的 `pImpl` 指针，必须在类的头文件中声明特种成员函数，在类的实现文件中实现它们
- 上述建议只针对 `unique_ptr` 并不适用于 `shared_ptr`

`Pimpl ` 意思是指涉到实现的指针。这种技巧就是把某类的数据成员用一个指涉到某实现类的指针替代，然后把原来在主类中的数据成员放置到实现类中，并使用指针间接访问这些数据成员

```c++
class Widget {	// 位于头文件 "widget.h"
public:
  Widget();
private:
  std::string name;
  std::vector<double> data;
  Gadget g1, g2, g3;
}
```

上述的 `Widget` 类需要依赖 `<string>`、`<vector>`、`"gadget.h"`，这些文件增加了客户的编译器时间。此外也使得客户依赖于于这些头文件，加入某个头文件的内容发生了改变，则 `Widget` 类的客户必须重新编译

在 `C++98` 中可以用一个指涉到已声明但是未定义的结构的裸指针来替换 `Widget` 的数据成员

```c++
// widget.h
class Widget {
public:
  Widget();
  ~Widget();
private:
  struct Impl;
  Impl* pImpl;
};

// widget.cc
#include "widget.h"
#include "gadget.h"
#include <string>
#include <vector>

struct Widget::Impl {
	std::string name;
  std::vector<double> data;
  Gadget g1, g2, g3;
};
Widget::Widget() : pImpl(new Impl) {}
Widget::~Widget() { delete pImpl; }
```

此时由于 `Widget` 不再使用 `std::string`、`std::vector` 和 `Gadget` 型别，因此 `Widget` 客户不再需要依赖这些型别的头文件。这会使编译速度提升，同时就算这些头文件内容发生变更，`Widget` 的客户也不会受到影响

> 一个已声明但是未定义的型别成为非完整型别

`Pimpl` 习惯用法的第一部分，是声明一个指针型别的数据成员，指涉到一个非完整型别。`Pimpl` 习惯用法的第二部分是动态分配和回收持有从前在原始类里的那些数据成员对象，分配和回收的代码放在实现文件中

下面是使用 `unique_ptr` 替代裸指针的代码

```c++
// widget.h
class Widget {
public:
  Widget();
private:
  struct Impl;
  std::unique_ptr<Widget> pImpl;
};

// widget.cc，省略 include
struct Widget::Impl {
  std::string name;
  std::vector<double> data;
  Gadget g1, g2, g3;
};
WIdget::Widget() : pImpl(std::make_unique<Impl>()) {}
```

注意上述代码没有声明和定义 `Widget` 的析构函数，这会当用户使用 `widget.h` 的时候导致问题。原因是由客户端的 `Widget` 对象析构引起的。由于没有定义析构函数，那么编译器会为我们生成析构函数，在编译器生成的默认析构函数内部会使用 `pImpl` 的默认析构器去析构内部的裸指针，默认析构器使用的是 `delete` 运算符，然而在实施 `delete` 运算符之前，`C++11` 会使用 `static_assert` 去确保裸指针未指涉到非完整型别。这么一来当编译器析构 `Widget` 对象的时候就会遇到一个失败的 `static_assert`

为了解决这个问题只需要保证生成析构 `std::unique_ptr<Widget::Impl>` 处，`Impl` 是一个完整型别即可。所以要让编译器看到 `Widget` 的析构函数函数体的位置在 `Widget::Impl` 定义之后

```c++
// widget.cc
struct Widget::Impl {/*...*/};
Widget::~Widget() = default;
```

> 对于移动操作来说也会产生由析构函数导致的问题。对于移动构造函数，会在其异常处理代码中生成析构 `pImpl` 的代码。而对于移动赋值运算符貌似没有问题，但是书上说有问题。解决移动操作导致的错误，同样也是把移动操作的定义放在 `widget.cc` 文件中，并放在析构函数的后面

上述的问题都是针对 `unique_ptr` 的，而对于 `shared_ptr` 就不会出现这种问题。原因在于 `unique_ptr` 和 `shared_ptr` 两种智能指针在实现 `pImpl` 指针行为时，对于自定义析构器的支持不同

对于 `unique_ptr` 来说，析构器是型别的一部分，这使得编译器生成更小尺寸的运行期数据结构以及更快的运行期代码，如此高效的后果是如果想要使用编译器生成的特种函数，就必须要求其指涉到的型别是完整的。可以理解为要生成更快的运行期代码，那么在编译期就需要做更多的检查

而对于 `shared_ptr` 而言，析构器并不是智能指针型别的一部分，所以就需要更大尺寸的运行期数据结构和更慢一些的代码，所以在使用编译器生成的特种函数的时候，其指涉到的型别不必是完整的

```c++
// widget.h
// 注意这里没有声明析构函数，也不会在 widget.cc 中定义析构函数
// 所以采用了编译器生成的默认析构函数
class Widget {
public:
  Widget();
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl;
};
```

