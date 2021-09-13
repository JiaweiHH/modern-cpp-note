## auto 的优点

- 只有编译器知道的类型
- 型别捷径

### 只有编译器知道的类型

有些类型只有编译器知道，我们无法指出这种类型，例如 闭包。这种情况下使用 `auto` 可以避免知道它的类型

```c++
auto derefUPLess = [] {
  std::cout << "hello\n";
}
```

对于闭包类型，我们虽然也可以使用 `std::function` 包装，但是 `std::function` 包装产生对象有如下的缺点：

1. 使用 `std::function` 存储的闭包变量是一个 `std::function` 实例，所以不管给定的签名如何它都占用着固定大小的内存。如果这个内存对于存储的闭包而言不够用的话，就回去堆上分配内存来存储该闭包。**所以 `std::function` 一般都会比使用 `auto` 声明的变量使用更多的内存**
2. 通过 `std::function` 来调用闭包几乎必然会比通过使用 `auto` 声明的  变量来调用闭包来的要慢

所以 `std::function` 通常比 `auto` 来的又慢又大，还可能导致内存耗尽异常

### 型别捷径

**第一个例子**

```c++
std::vector<int> v;
unsigned sz = v.size();
```

`v.size()` 的类型是 `std::vector<int>::size_type`。如果我们直接使用 `unsigned` 来接收该变量，对于 32 位 Windows 来说 `unsigned` 是 32 位，对于 64 位 Windows 来说 `unsigned` 是 64 位，这意味着在 32 位运行正常的代码在 64 位上可能会表现异常

> 使用 `auto` 可以避免这种移植问题

**第二个例子**

```c++
std::unorder_map<std::string, int> m;
for(const std::pair<std::string, int>& p : m) {
  // ...
}
```

这里值得一提的是 `std::unorder_map` 存储的键值对实际上是 `<const std::string, int>`，但是我们的循环变量是 `std::pair<std::string, int>`，所以编译器就需要将 `<const std::string, int>` 转化成 `<std::string, int>`。

这需要对 `m` 中的每个对象做一次复制操作，形成一个 `p` 想要绑定的型别的临时对象，然后把 `p` 绑定到这个对象，在每次迭代结束的时候该临时变量都会被析构一次

> 使用 `auto` 就可以带来效率上的提升

上面两个型别捷径的问题说明了，显示指定型别可能会导致不想要的、也没有想到的隐式型别转换

## auto 的缺点

- 型别推导的时候对于 `auto a = {1}` 会推导成 `std::initializer_list`
- 隐形的的代理型别可以导致 `auto` 根据初始化表达式推导出 “错误的” 型别，对推导得出的变量的使用可能会出现未定义的行为（例如空悬指针）

`std::vector<bool>::reference` 就是一个很典型的隐形代理型别

```c++
Widget w;
auto highPriority = features()[5];	// features() 返回 std::vector<bool>
processWidget(w, highPriority);
```

1. `std::vector<bool>::operator[]` 返回值是 `std::vector<bool>::reference` 的临时对象称为 `temp`，该对象含有一个指涉到机器字的指针以及偏移
   - 该返回值型别的实现方式可能是，内涵了一个指针指向了一个字，并且含有对应 `index` 比特位在字中的偏移值
2. `highPriority` 是 `std::vector<bool>::reference` 对象的一个副本，所以 `highPriority` 也含有一个指涉到 `temp` 中的机器字的指针（以及偏移）
3. 在表达式结束的地方，`temp` 会被析构，结果 `highPriority` 会含有一个空悬指针，在调用 `processWidget()` 的时候会出现未定义的行为

对此的解决方式是 `auto highPriority = static_cast<bool>(features()[5])` 进行强制类型转换，这么做的另一个好处是告诉表达自己的想法 “这里我想要一个 bool 类型的变量”

> 隐形的代理类无法和 auto 和平相处，所以要防止出现这样的代码 `auto someVar = 隐形代理型别表达式`

