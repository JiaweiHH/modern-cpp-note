## unique_ptr

- `std::unique_ptr` 是小巧的、高速的、具备只移型别的智能指针，默认情况下占用的空间和裸指针相同，并且会和裸指针执行相同的指令
- 资源的析构默认采用 `delete` 实现，但是也可以自定义析构器。有状态的析构器会增加 `unique_ptr` 型别的对象大小
- `unique_ptr` 可以很容易的转换为 `shared_ptr`

当需要使用指针的时候 `unique_ptr` 应该成为首选。下面是一个使用 `unique_ptr` 作为返回值的工厂函数，其中 `Stock`, `Bond`, `RealEstate` 类都继承 `Investment` 类

```c++
class Investment {
public:
  // ...
  virtual ~Investment() {} // 析构函数必须是虚函数
};

// 工厂函数
template <typename... Ts>
std::unique_ptr<Investment>
makeInvestment(Ts&&... params) {
  // 先创建一个空的 unique_ptr，默认使用 delete 作为析构器
  // 然后根据需要创建的具体型别，使 unique_ptr 指向具体型别对象
  std::unique_ptr<Investment> pInv(nullptr);
  if(/* 应该创建 Stock 型别的对象 */) {
    pInv.reset(new Stock(std::forward<Ts>(params)...));
  } else if(/* 创建 Bond 型别的对象 */) {
    pInv.reset(new Bond(std::forward<Ts>(params)...));
  } else if(/* 创建 RealEstate 型别对象 */) {
    pInv.reset(new RealEstate(std::forward(params)...));
  }
  return pInv;
}
```

另外 `unique_ptr` 可以自定义析构器，并且如果选择使用无状态的 `lambda ` 表达式作为析构器的话是不会占用额外的空间的（无状态是指：没有捕获），普通函数会具有一定的状态因此会占用额外空间

```c++
// 使用无状态 lambda 表达式作为自定义析构器
auto delInvmt1 = [](Investment* pInvestment) {
  makeLogEntry(pInvestment);	// 该函数负责记录日志
  delete pInvestment;
}
template <typename... Ts>
std::unique_ptr<Investment, decltype(delInvmt1)>
makeInvestment(Ts&&... args);	// 返回尺寸与 Investment* 的尺寸相同

// 使用普通函数作为自定义析构器
void delInvmt2(Investment* pInvestment) {
  makeLogEntry(pInvestment);
  delete pInvestment;
}
template <typename... Ts>
std::unique_ptr<Investment, void (*)(Investment*)>
makeInvestment(Ts&&... args);	// 返回尺寸 = Investment* 的尺寸 + 函数指针的尺寸
```

`unique_ptr` 还有一个特性就是它可以高效的转化为 `shared_ptr`，所以对于工厂函数来说一般都是返回 `unique_ptr`

```c++
std::shared_ptr<Investment> sp = makeInvestment(args);
```

