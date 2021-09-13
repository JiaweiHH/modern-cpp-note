#include <iostream>
#include <type_traits>

void foo(char *) {
  std::cout << "foo(char *) is called\n";
}

void foo(int i) {
  std::cout << "foo(int) is called\n";
}

int main() {
  /* 下面三个全都不相等 */
  if(std::is_same<decltype(NULL), decltype(0)>::value)
    std::cout << "NULL == 0\n";
  if(std::is_same<decltype(NULL), decltype((void *)0)>::value)
    std::cout << "NULL == (void *)0\n";
  if(std::is_same<decltype(NULL), std::nullptr_t>::value)
    std::cout << "NULL == nullptr\n";

  foo(0); // call int
  // foo(NULL)  // 该行不能通过编译
  foo(nullptr); // call char*
}