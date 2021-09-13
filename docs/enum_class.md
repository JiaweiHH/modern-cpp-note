## enum class

- 限定作用域的枚举型别只在枚举型别内可见，只能通过强制型别转换以转换成其他型别
- 限定作用域枚举和不限定作用域枚举都支持指定底层型别
  - 限定作用域的枚举型别其底层型别默认是 `int`
  - 不限定作用域的枚举型别没有默认底层型别
- 不限定作用域的枚举型别只能在指定底层型别的情况下才可以进行前置声明

传统的 `enum` 有一个致命的缺点就是枚举量的名字属于这个枚举类型的作用域，这就意味着在此作用域内不能有其他实体取相同的名字

```c++
enum Color { black, white, red };
auto white = false;	// 错误. white 已经被声明过了
```

在 `C++11` 中出现了限定作用域的枚举型别 `enum class`。`enum class` 除了作用域被限定之外，还有一个特点就是它的枚举量不会进行任何途径的隐式类型转换。而不限定作用域的枚举型别是可以进行隐式类型转换的

```c++
// valuei 的类型为 new_enum&&，其底层型别是 unsigned int
enum class new_enum : unsigned int {
  value1,
  value2,
  value3 = 100,
  value4 = 100
};

// 错误，不能直接获取枚举类型的值. 枚举值不会进行任何的隐式类型转换
// unsigned int a = new_enum::value3;
```

限定作用域的枚举型别和不限定作用域的枚举型别都可以进行前置声明。`enum class` 拥有默认的底层存储型别 `int`，而 `enum` 的底层存储是根据其枚举值来确定的。但是我们也可以手动指定两种枚举型别的底层类型，所以 `enum` 如果想要前置声明则必须要先手动指定底层存储型别，否则编译器不知道底层存储型别声明会出错

由于 `enum` 的底层存储型别默认是编译器根据枚举值来确定的，那就意味着当我们修改了枚举值的时候可能整个系统都需要重新编译，即使只有一个子系统用到了更改的枚举值。而使用 `enum class` 没有这个问题，因为底层型别是确定的

```c++
enum Status { 
  value1 = 0, 
  value2 = 1,
  value3 = 500,
  value4 = 0xFFFFFFFF	// 新增的枚举值，可能会导致枚举值的底层型别变化
};
```

不过有时候 `enum class` 并不是都比 `enum` 来的简单，例如：当需要引用 `std::tuple` 的各个域时。使用不限定作用域的枚举型别可以直接将枚举值隐式类型转换到 `std::size_t`

```c++
using UserInfo = std::tuple<std::string, std::string, std::size_t>;
enum UserInfoFields_1 { uiName, uiEmail, uiReputation };

UserInfo uInfo;
auto val = std::get<uiEmail>(uInfo);	// std::get<> 的模版形参是 std::size_t，并且需要编译期常量
```

而使用限定作用域的枚举型别则显得不是那么简单，因为枚举值不能隐式类型转换到 `std::size_t`。我们需要写一个配合任何枚举型别的函数模版，并且函数是 `constexpr` 的。由于形参可以是任何枚举型别，那么返回值就不能是 `std::size_t` 了，必须是枚举值的底层型别

```c++
enum UserInfoFields_2 { uiName_2, uiEmail_2, uiReputation_2 };
// auto val = std::get<UserInfoFields::uiEmail_2>(uInfo);	// 错误

template<typename T>
constexpr std::underlying_type_t<T> toUType(T enumerator) noexpect {
	return static_cast<std::underlying_type_t<T>>(enumerator);
}
auto val = std::get(toUType(UserInfoFields::uiEmail_2))
```

> 最后如果想要输出 `enum class` 的枚举值，需要重载 `operator<<`

```c++
// std::enable_if<a, b> 如果 a 成立就在 std::enable_if<a, b> 中 typedef b 类型
template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, T e) {
  return stream << static_cast<typename std::underlying_type<T>::type>(e);
}
```