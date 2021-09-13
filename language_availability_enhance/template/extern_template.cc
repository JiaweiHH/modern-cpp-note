#include <vector>

template class std::vector<bool>; /* 会在当前编译单元强制实例化 */
extern template class std::vector<double>;  /* 不在当前编译单元中实例化模版，C++11 引入 */