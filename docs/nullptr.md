## nullptr

表示空指针有三种形式：`0`, `NULL`, `nullptr`

- 字面常量 0 的型别时 `int`，而非指针。在 C++ 中发现了一个字面常量 0，也会勉强解释为空指针
- `NULL` 与字面常量 0 类似，只不过 `NULL` 可以赋值为非 `int` 的整型型别例如 `0L`
- `nullptr` 不具备整型型别，但是它也不具备指针型别（这一点字面常量 0 和 `NULL` 也是一样的）
  - `nullptr` 的型别是 `std::nullptr_t`，该型别可以隐式转换到所有的裸指针型别

```c++
/* 下面三个全都不相等 */
if(std::is_same<decltype(NULL), decltype(0)>::value)
  std::cout << "NULL == 0\n";
if(std::is_same<decltype(NULL), decltype((void *)0)>::value)
  std::cout << "NULL == (void *)0\n";
if(std::is_same<decltype(NULL), std::nullptr_t>::value)
  std::cout << "NULL == nullptr\n";
```

使用 `nullptr` 的好处在于：

1. 当需要使用空指针的时候，代码易读性较高
2. 避免了函数重载决议的意外

```c++
void f(int);
void f(bool);
void f(void*);
f(0);       // 调用 f(int)
f(NULL);    // 可能不能通过编译
f(nullptr);	// 调用 f(void*)
```

上面的例子中 `f(0)` 的调用以及 `f(NULL)` 的调用可能都会出乎我们的意外。另外当在模版函数中传入空指针时尤其需要注意

```c++
int f1(std::shared_ptr<Widget> spw);
double f2(std::unique_ptr<Widget> upw);
bool f3(Widget *pw);

template<typename FuncType, typename MuxType, typename PtrType>
decltype(aut0) lockAndCall(FuncType func, MuxType& mutex, PtrType ptr) {
  std::mutex_guard<std::mutex> g(mutex);
  return func(ptr);
}

std::mutex mutex;
auto res1 = lockAndCall(f1, mutex, 0);        // 错误
auto res2 = lockAndCall(f2, mutex, NULL);     // 错误
auto res3 = lockAndCall(f3, mutex, nullptr);  // 正确
```

`res1` 的获取会出现问题，因为传入 `0` 所以模版参数 `PtrType` 被推导为 `int`，而在调用 `f1` 的时候不允许 `int` 转换为 `std::shared_ptr`

`res2` 的获取也会出现问题，原因与 `res` 基本相同，不允许整型型别转化为 `std::unique_ptr`

`res3` 的获取不会出现问题，`PtrType` 被推导为 `std::nullptr_t` 而该类型可以隐式转化为任何型别的指针