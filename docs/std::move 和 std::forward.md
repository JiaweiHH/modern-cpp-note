## std::move 和 std::forward

- `std::move` 执行到右值的无条件转换，但就自身而言，它不移动任何东西
- `std::forward` 只有当它的参数被绑定到一个右值的时候，才将参数转换为右值
- `std::move` 和 `std::forward` 在运行期什么也不做

首先需要知道的是：

1. `std::move` 不移动任何东西，`std::forward` 也不转发任何东西
2. 在运行期间它们不做任何事情，它们不会产生代码，一个字节都没有

`std::move` 和 `std::forward` 仅仅是执行转换的函数模版，`std::move` 无条件的将它的参数转换为右值，`std::forward` 在特定情况满足时进行转换

### std::move

```c++
template <typename T>
typename remove_reference<T>::type&&
move(T&& param) {
  using ReturnType = typename remove_reference<T>::type&&;
  return static_cast<ReturnType>(param);
}
```

`std::move` 的大致实现方式如上所示，可以看到它除了将它的参数转换到右值以外什么也不做，所以记住 `std::move` 并不移动任何东西。对一个对象使用 `std::move` 实际上是告诉编译器：这个对象很适合被移动，这也是为什么 `std::move` 叫现在的名字

另外，右值只不过经常是移动操作的候选者，但是并不是所有的右值都是可以移动的。考虑下面这个例子

```c++
class Annotation {
public:
  explict Annotation(const std::string text) : value(std::move(text));
private:
  std::string value;
};
```

上面这段代码可以编译、可以链接、也可以运行，这段代码期望将 `text` 中的值移动到 `value`，但是实际的执行结果是复制而不是移动。原因在于：

1. `text` 的型别是 `const std::string&`，所以在执行 `std::move(text)` 之后返回的型别是 `const std::string&&`。`const` 属性一直保留着
2. 由于移动构造函数只接受一个指向非常量的右值引用，所以 `const std::string&&` 的型别不会传递给移动构造函数
   - 移动构造函数不接受 `const` 右值是合理的，因为移动之后可能需要修改原来对象的值
3. 复制构造函数由于其形参是 `const std::string&` 可以绑定到右值，所以最后该型别会被传递给复制构造函数

上面这个例子说明了：不要在你希望能移动对象的时候声明它们为常量，以及 `std::move` 不仅不移动任何东西而且它也不保证它执行转换的对象可以被移动

> `std::move` 唯一可以确保的一件事是它应用到一个对象，能够得到该对象的右值
>
> 这里有一个值得思考的问题是：为什么 `std::move` 不返回一个 `T` 型别，因为返回一个值对象也会被视为为右值

### std::forward

`std::forward` 和 `std::move` 是相似的。但是 `std::forward` 只在满足一定条件下才执行转换，它只把右值初始化的参数转换为右值。

关于如何确定该参数是左值初始化的还是右值初始化的，考虑下面这个例子

```c++
void process(const Widget& lvalArg);  // 左值处理
void process(Widget&& rvalArg);       // 右值处理
template <typename T>
void logAndProcess(T&& param) {
  auto now = std::chrono::system_clock::now();	// 获取现在时间
  makeLogEntry("calling 'process'", now);
  process(std::forward<T>(param));
}

Widget w;
logAndProcess(w);             // call with lvalue
logAndProcess(std::move(w));  // call with rvalue
```

在上述代码中，`std::forward` 是依靠模版形参 `T` 来分辨 `param` 是被左值初始化还是右值初始化的，当传递进来的参数是一个左值的时候 `T` 的型别是 `Widget&`，而当传递进来的参数是一个右值的时候 `T` 的型别是 `Widget`，具体的可以查看 [模版型别推导](./模版型别推导.md) 那一篇笔记

> 一般来说 `std::forward` 都会在模版函数中使用，并且该模版参数是一个万能引用，此时模版形参 `T` 可以分辨出是右值初始化还是左值初始化

`std::forward` 的大致实现如下

```c++
template<typename _Tp>
constexpr _Tp&& lmitate_forward(typename std::remove_reference<_Tp>::type& __t) noexcept
{ return static_cast<_Tp&&>(__t); }

template<typename _Tp>
constexpr _Tp&& lmitate_forward(typename std::remove_reference<_Tp>::type&& __t) noexcept {
  static_assert(!std::is_lvalue_reference<_Tp>::value, "template argument _Tp is an lvaue reference type");
  return static_cast<_Tp&&>(__t);
}
```

最后在这里补充一点：

- 右值引用作为返回值的时候它是右值，但是作为变量的时候（也就是出现在等号左边）它就是左值
- 左值引用不管是在等号的左边，还是作为变量的返回值，它都是左值

在 [perfect_forwarding.cc](../language_runtime_enhance/rvalue_reference/perfect_forwarding.cc) 还有更详细的代码示例
