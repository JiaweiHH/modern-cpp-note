## weak_ptr

- 使用 `weak_ptr` 来代替可能空悬的 `shared_ptr`
- `weak_ptr` 的可能用武之地包括缓存、观察者列表、以及避免 `shared_ptr` 引起的指针环路等
- `weak_ptr` 的开销和 `shared_ptr` 是差不多的

`weak_ptr` 不是一种独立的智能指针，而是 `shared_ptr` 的一种扩充。`weak_ptr` 不能进行指针提领运算，也不会增加共享对象的引用计数

`weak_ptr` 可以通常通过 `shared_ptr` 来创建，并且 `weak_ptr` 的 `expired()` 函数可以检查指针是不是已经失效

> 注意指针值为 nullptr 并不能判断该指针是空悬的

```c++
// 使用 shared_ptr 来创建 weak_ptr

auto spw = std::make_shared<Widget>();
std::weak_ptr<Widget> wpw(spw);	// wpw 和 spw 指涉到同一个 Widget，引用计数仍旧为 1
spw = nullptr;	// 引用计数变为 0，wpw 空悬
if(wpw.expired())
  /*...*/
```

同样的 `weak_ptr` 也可以用来创建 `shared_ptr`，有两种方法可以做到。一种形式是使用 `weak_ptr::lock()`，它会返回一个 `shared_ptr`，如果此时的 `weak_ptr` 已经失效则创建的 `shared_ptr` 为空

```c++
std::shared_ptr<Widget> spw1 = wpw.lock();
auto spw2 = wpw.lock();
```

另一种方式是，以 `weak_ptr` 为实参来构造 `shared_ptr`。这个时候如果 `weak_ptr` 失效的话会抛出异常

```c++
// 如果 wpw 失效，抛出 std::bad_weak_ptr 型别的异常
std::shared_ptr<Widget> spw3(wpw);
```

`weak_ptr` 的一个实用的场景是 `cache`，缓存需要检查自己保存的条目是不是已经失效。下面是一个带缓存的工厂函数 `fastLoadWidget()`，这里工厂函数是 `loadWidget()`，并且返回值型别是 `shared_ptr` 因为需要返回值到 `weak_ptr` 的转化，所以不能返回 `unique_ptr`

```c++
std::shared_ptr<const Widget> fastLoadWidget(WidgetID id) {
  static std::unordered_map<WidgetID, std::weak_ptr<Widget>> cache;
  auto objPtr = cache[id].lock();
  if(!objPtr) {
    objPtr = loadWidget(id);  // objPtr = nullptr 则创建一个新的
    cache[id] = objPtr;       // 从 shared_ptr 创建 weak_ptr 
  }
  return objPtr;
}
```

另外观察者模式也适用 `weak_ptr`。`subject` 持有指涉到其观察者的指针，因此需要确定该指针是否已经空悬。

最后考虑如下的场景，`A` 和 `C` 持有 `B` 的共享指针，此时如果 `B` 需要持有 `A` 的指针，那么 `weak_ptr` 是首选

- 如果使用裸指针。如果 A 被析构，而 C 仍然指涉到 B，B 将保留着指涉到 A 的空悬指针，但是 B 却无法检测
- 如果使用 `shared_ptr`。这种情况会导致 A 和 B 互相持有对方的引用计数，导致谁都无法被释放，最后 A 和 B 无法被析构，资源泄漏
- 如果使用 `weak_ptr`。避免了上述两个问题。加入 A 被析构，B 的指针将会空悬，但是 B 可以检测到。如果 A 和 B 互相指涉到彼此，但是 B 的指针不会增加 A 的引用计数，所以不会阻止 A 被析构

从效率上看 `weak_ptr` 和 `shared_ptr` 本质上是一致的。`weak_ptr` 和 `shared_ptr` 的对象尺寸相同，它和 `shared_ptr` 使用同样的控制块，其构造、析构、赋值操作都包含了对引用计数的原子操作

> 对上面这段话的注释：`weak_ptr` 不会干涉对象的共享所有权，因此不会影响对象的引用计数。但是实际上控制块里面还有第二个引用计数，而 `weak_ptr` 操作的就是第二个引用计数
