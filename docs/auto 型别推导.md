## auto 型别推导

`auto` 型别推导就是模版型别推导，除了一个特殊的例子——对于 `initializer_list` 的型别推导，模版是不能直接推导初始化列表的，而 `auto` 可以推导

```c++
auto x{27};	// x 的型别不是 int，而是 std::initializer_list<int>

template<typename T>
void f(T param)

f({1, 2, 3});	// 错误，模版不能推导初始化列表
```

虽然模版不能推导初始化列表，但是我们却可以把函数模版的形参设置为 `std::initializer_list<T>`，这样模版可以推导了

另外在 `C++14` 中允许使用 `auto` 来说明函数的返回值，而且 `C++14` 中的 lambda 表达式也可以在形参中使用 `auto`。对于这两种 `auto` 的用法是属于模版型别推导的范畴，而不是 `auto` 型别推导的范畴