#include <vector>
#include <string>

template<typename T, typename U>
class MagicType {
public:
  T dark;
  U magic;
};

// 下面这种写法是不合法的，模版不是一个类型 而是用来产生一个类型的，不能使用 typedef
// template<typename T>
// typedef MagicType<std::vector<T>, std::string> FakeDarkMagic;

/* typedef 和 using 对于函数指针的别名定义 */
typedef int (*process)(void *);
using NewProcess = int(*)(void *);

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