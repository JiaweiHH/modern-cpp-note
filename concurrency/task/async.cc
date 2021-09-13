#include <future>
#include <iostream>
#include <thread>

// 封装 std::async，确保函数 f 会异步执行
template <typename F, typename... Ts>
inline std::future<std::result_of<F(Ts...)>>
really_async(F &&func, Ts &&...params) {
  return std::async(std::launch::async, std::forward<F>(func),
                    std::forward<Ts>(params)...);
}