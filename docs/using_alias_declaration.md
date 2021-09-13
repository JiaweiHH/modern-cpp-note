## using 别名声明

- `typedef` 不支持模版化，但是别名支持
- 别名模版可以避免写 `::type` 后缀，并且在模版内对于内嵌 `typedef` 的引用需要加上 `typename` 前缀

`typedef` 和 `using` 在很多情况下都是可以互相使用的，例如声明一个函数指针

```c++
/* typedef 和 using 对于函数指针的别名定义 */
typedef int (*process)(void *);
using NewProcess = int(*)(void *);
```

但是当遇到模版的时候无法使用 `typedef` 来声明模版的别名，而 `using` 照样可以继续使用。不过使用 `typedef` 照样可以做到 `using` 要表达的意思

```c++
template<typename T, typename U>
class MagicType {
public:
  T dark;
  U magic;
};

// 下面这种写法是不合法的，模版不是一个类型 而是用来产生一个类型的，不能使用 typedef
// template<typename T>
// typedef MagicType<std::vector<T>, std::string> FakeDarkMagic;

/* C++11 引入模版别名 */
template<typename T>
using TrueDarkMagic = MagicType<std::vector<T>, std::string>;
TrueDarkMagic<int> type1;

/* typedef 一样可以做到 using 要表达的意思 */
template<typename T>
struct typdef_template {
  typedef MagicType<std::vector<T>, std::string> type;
};
typdef_template<int> type2;
```

虽然使用嵌套在模版化的 `struct` 里面的 `typedef` 也能做到模版的别名，但是如果在一个模版中使用 `typedef` 的模版别名需要在前面加上 `typename` 前缀，而在模版中使用 `using` 模版别名则不需要。

这是因为当编译器处理到 `Widget_1` 并看到 `TrueDarkMagic<T>` 的时候，它知道 `TrueDarkMagic<T>` 是一个型别的名字，因为 `TrueDarkMagic` 是一个别名模版那就必然命名了一个型别。而当编译器遇到 `Widget_2` 里面的 `typdef_template<T>::type` 时并不能确定 `typdef_template<T>::type` 命名了一个型别

> C++ 规定带依赖型别必须前面加上 `typename`

```c++
// 在模版中使用 using 的模版别名
template<typename T>
class Widget_1 {
private:
  TrueDarkMagic<T> data;
};
// 在模版中使用 typedef 完成的模版别名
template<typename T>
class Widget_2 {
private:
  typename typdef_template<T>::type data;
};
```

其实我们可以很轻松的对 `typedef` 定义的模版别名使用 `using` 从而避免上述的麻烦。`C++14` 中对于 `C++11` 里面提供的型别特征就是这么处理的，`C++11` 的型别特征是采用 `typedef` 实现的，到了 `C++14` 提供了 `using` 的版本

```c++
template<typename T>
using remove_const_t = typename remove_const<T>::type;

template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

template<typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
```

