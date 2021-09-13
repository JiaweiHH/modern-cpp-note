现代 C++ 特性的学习笔记，包含 C++11/14/17 一些常用的功能，主要是为了以后回顾使用

目录根据《Effective Modern C++》整理，代码目录按照《modern-cpp-tutorial》整理。有一些内容两本书重复的我会在 `markdown` 中包含了 `.cc` 里面的部分实现代码，另外部分只有《modern-cpp-tutorial》特有的部分参考 `.cc` 代码部分的注释

- 第 I 部分：型别推导
  - [模版型别推导](docs/模版型别推导.md) 
  - [auto 型别推导](docs/auto%20型别推导.md)
  - [decltype](docs/decltype.md)
  - [查看型别推导方法.md](docs/查看型别推导方法.md)
- 第 II 部分：auto
  - [auto](docs/auto.md)
- 第 III 部分：现代 C++
  - [对象初始化方式](docs/对象初始化.md)
  - [使用 nullptr](docs/nullptr.md)
  - [using 别名声明](docs/using_alias_declaration.md)
  - [限定作用域的枚举型别](docs/enum_class.md)
  - [删除函数](docs/删除函数.md)
  - [引用饰词和 override](docs/引用饰词和%20override.md)
  - [const 迭代器](docs/const_iterator.md)
  - [constexpr](docs/constexpr.md)
  - [特殊成员函数的生成机制](docs/特殊成员函数的生成机制.md)
- 第 IV 部分: 智能指针
  - [unique_ptr](docs/unique_ptr.md)
  - [shared_ptr](docs/shared_ptr.md)
  - [weak_ptr](docs/weak_ptr.md)
  - [智能指针的 make function](doncs/../docs/make_shared%20和%20make_unique.md)
  - [Pimpl 惯用法](docs/Pimpl.md)
- 第 V 部分: 右值引用、移动语意和完美转发
  - [std::move & std::forward](docs/std::move%20和%20std::forward.md)
  - [通用引用.md](docs/通用引用.md)
  - [关于何时使用 std::move 和 std::forward](docs/关于何时使用%20std::move%20和%20std::forward.md)
  - [重载通用引用的问题](docs/通用引用和重载组合的问题.md)
  - [通用引用和重载组合的替代方法和解决方法](docs/通用引用和重载组合的替代方法和解决方法.md)
  - [引用折叠](docs/引用折叠.md)
  - [无法进行完美转发的类型](docs/无法进行完美转发的类型.md)
- 第 VI 部分: lambda 表达式
  - [lambda](docs/lambda.md)