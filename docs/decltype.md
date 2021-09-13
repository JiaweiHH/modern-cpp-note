## decltype

`decltype` 可以用在函数的尾置返回类型，`decltype` 会返回表达式的完整类型，`decltype(auto)` 可以实现完整的类型推导

```c++
Widget w;
const Widget& cw = w;
auto myWidget1 = cw;	// 型别是 Widget
decltype(auto) myWidget2 = cw;	// 型别是 const Widget&
```

**但是 `decltype` 偶尔也会不符合我们的预期**——`decltype` 如果运用在仅仅比单纯的名字更加复杂的左值表达式的话，`decltype` 得出的类型就总是左值引用。`decltype(auto)` 也会出现这种情况

```c++
int x;
decltype(x) y;	// y 的型别是 int
decltype((x)) z;	// z 的型别是 int&，因为 () 表达式的返回值也是一个左值

decltype(auto) f1() {
  int x = 0;
  return x;		// 返回值是 int
}
decltype(auto) f2() {
  int x = 0;
  return (x);	// 返回值是 int&，这是非常危险的，因为函数返回后 x 会被销毁，引用出现未定义行为
}
```

> `decltype(auto)` 是用在函数返回值或者变量声明上面的，在变量声明的时候用初始化表达式替换 `auto` 然后执行 `decltype` 的推导规则

