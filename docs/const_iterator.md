## const_iterator

- 优先使用 `const_iterator` 而非 `iterartor`
- 在最通用的代码中，优先使用非成员函数版本的 `begin`, `end` 和 `rbegin` 等

任何时候只要你需要一个迭代器而其指涉到的内容没有必要修改，就应该使用 `const_iterator`

容器的 `cbegin()` 和 `cend()` 都返回 `const_iterator` 型别，就算容器本身是非 `const` 的也是一样

```c++
// 在容器中查找 1983 的位置，然后插入 1998
std::vector<int> values;
auto it = std::find(values.cbegin(), values.cend(), 1983);
values.insert(it, 1998);
```

某些容器或者某些类似容器的数据结构并没有成员函数 `begin()`, `end()`, `cbegin()`, `cend()` 等

```c++
// 设计通用的插入函数，其中使用 const_iterator 来完成插入
template<typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVal) {
  // C++14 提供 cbegin(), cend()
  using std::cbegin;
  using std::cend;
  auto it = std::find(cbegin(container), cend(container), targetVal);
  container.insert(it, insertVal);
}
```

`C++11` 中并没有 `std::cbegin()` 和 `std::cend()`，只有 `std::begin()` 和 `std::end()`。使用 `begin()` 和 `end()` 也可以获取 `const_iterator`，这是由于当参数是 `const` 容器的时候这两个函数会返回 `const` 迭代器

```c++
// 即便实参不是 const 容器，这里只需要用 const 引用来接收就可以了
template<typename C>
decltype(auto) cbegin(const C& container) {
  return std::begin(container);
}
```

> 对于内建数组来说，`begin` 和 `end` 会返回指向首元素和尾后元素的指针，`cbegin` 和 `cend` 同理