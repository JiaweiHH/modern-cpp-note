## constexpr

`constexpr` 可以在对象和函数上使用。当它应用于对象时该对象不仅具有 `const` 属性，并且在编译期就知道其值；当它应用于函数时会根据函数的实参具有不同的意义

首先说明一些关于 `const` 和 `constexpr` 的概念：

1. `const` 的对象可能是一个编译期常量也可能不是
   - 例如 `int a = 1; const int b = a` 则 `b` 就不是一个编译期常量
   - 而 `const int b = 1` 则 `b` 就是一个编译期常量
2. `constexptr` 对象只能使用编译期常量来初始化
3. 常量表达式和编译期常量等价

```c++
int sz = 10;	// 非 const 变量
constexpr auto arraySize1 = sz;	// 错误，sz 的值在编译期未知
constexpr auto arraySize2 = 10;	// 正确
std::array<int, sz> data1;	// 错误，array 要求数组大小在编译期已知
std::array<int, arraySize2> data2;	// 正确

const auto arraySize3 = sz;	// 没有问题，但是 arraySize3 在编译期未知
const auto arraySize4 = 10;	// arraySize4 在编译期已知
std::array<int, arraySize3> data3;	// 错误
std::array<int, arraySize4> data4;	// 正确
```

另外 `constexpr` 也可以用在用户自定义的类上，但是

1. 要求该对象的构造时候使用的构造函数必须是 `constexpr` 的
   - `constexpr` 构造函数所产生的对象中的所有成员都是 `constexpr` 的，所以直接通过作用域运算符获取的对象就是 `constexpr` 的
2. `constexpr` 对象调用非 `constexpr` 函数，即便这个函数返回的是成员变量（前面提到 `constexpr` 对象的成员变量是 `constexpr` 的），该函数的返回值也不是编译期常量
   - 这很好理解，毕竟返回的只是一个副本

```c++
class Widget {
public:
  constexpr Widget(size_t _a, size_t _b) : a(_a), b(_b) {}
  constexpr Widget() : a(5), b(5) {}
  size_t get_a() const { return a; }
  constexpr size_t get_b() const { return b; }
  size_t a;
  size_t b;
};

constexpr Widget w1{10, 20};	// 正确
// constexpr Widget w1;	// 错误，默认构造函数不是 constexpr 的

std::array<int, w1.a> arr1;	// 正确
std::array<int, w1.get_b()> arr2;	// 正确
// std::array<int, w1.get_a()> arr3;	// 错误
```

> 在 `C++11` 中 `constexpr` 成员函数默认具有 `const` 属性，并且在 `C++11` 中 `void` 并不是一个字面型别，这两条限制在 `C++14` 中被移除了。所以在 `C++14` 中允许更改成员值的成员函数为 `constexpr` 的

上述都是 `constexpr` 作用在对象上面的情况，`constexpr` 也可以作用在函数上。当 `constexpr` 作用在函数上，它展现的效果是：

1. 如果传递给 `constexpr` 函数的实参是编译期常量，那么函数的返回值也会是一个编译期常量
2. 如果传递给 `constexpr` 函数的实参有一个或者多个在编译期未知，则它的运作方式和普通函数一样
3. `constexpr` 函数仅限于传入和返回字面型别，意思是这样的型别能够在编译期持有可以决议的值，所有的内建型别都是字面型别。

这也意味着我们不需要针对两种使用场景设计两个函数，一个 `constexpr` 函数本身就能够应用于两个场景

```c++
// C++11 中 constexpt 不得包含多余一个执行语句，但可以使用递归和 ?: 运算符
constexpr int pow(int base, int exp) {
  return (exp == 0 ? 1 : base * pow(base, exp - 1));
}
// C++14 中取消了上面的限制
constexpr int pow(int base, int exp) {
  auto result = 1;
  for(int i = 0; i < exp; ++i)
    result *= base;
 	return restlt;
}
// 如果 constexpr 函数使用了一个用户自定义的对象，则该对象使用的构造函数必须是 constexpr 的
constexpr Widget func() {
  // Widget w{10, 20};	// 正确
  // Widget w{};	// 错误
  
  return Widget{10, 20};	// 正确
}
```

这里需要注意的是如果 `constexpr` 函数使用了一个用户自定义的对象，则该对象使用的构造函数必须是 `constexpr` 的，不管该函数用在什么场景都会编译出错

> 针对这里我的理解是：因为 `constexpr` 函数既可以运用于需要编译期常量的场景，也可以运用于普通的场景。首先对于内建型别没有这种限制，`C++14` 允许在 `constexpr` 函数中使用内建型别了，并且并不需要声明变量为 `constexpr`，所以我估计一旦函数用于需要编译期常量的场景编译器自己会做优化把它变成 `constexpr` 变量
>
> 但是如果一旦在函数里面使用了通过非 `constexpr` 构造函数构造出来的用户自定义对象时，则一旦当这个函数用于需要编译期常量的场景的时候，由于编译器使用这个构造函数优化不了，所以从源头上拒绝这种行为

最后一点，当 `constexpr` 函数用于需要编译期常量场景的时候，其中用户自定义型别所使用到的成员函数必须是 `constexpr` 的

```c++
constexpr Widget func() {
  Widget res{10, 20};
  Widget tmp{20, 30};
  int b = tmp.get_a();
  return res;
}
Widget res1 = func();	// 该语句编译没有问题
constexpr Widget res2 = func();	// 该语句编译出错
```

