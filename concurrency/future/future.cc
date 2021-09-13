#include <iostream>
#include <future>
#include <thread>

/**
 * @brief future 可以用来异步获取任务的结果
 * 自然的，很容易能够想象到把它作为一种简单的线程同步手段，barrier
 * 
 * 考虑这样一个场景，如果主线程 A 希望开辟一个新线程 B 去执行预期的任务，
 * 并给主线程一个结果，而这个时候 A 正在忙其他的事情，无暇顾及 B 的结果，
 * 所以 A 会希望能够在某一个特定的时间获取 B 的结果
 * 
 * 在 C++11 之前，通常的做法如下：
 * 1. 在 A 里面启动线程 B
 * 2. 当 B 准备完毕之后，发送一个事件，并将结果保存到全局变量
 * 3. 当 A 需要结果的时候，调用一个线程等待函数来获得执行的结果
 * 
 * C++11 引入了 future 可以简化上述操作
 */

int main() {
  /* 将一个 lambda 表达式封装到 task 中 */
  std::packaged_task<int()> task([] { return 7; });
  /* 获得 task 的期物 */
  std::future<int> result = task.get_future();
  /* 在一个线程中执行 task，主线程继续执行 */
  std::thread(std::move(task)).detach();
  std::cout << "waiting...";

  /* 在此设置屏障，阻塞到期物的完成 */
  result.wait();
  std::cout << "done!" << "\n";
  /* 获取期物的结果 */
  std::cout << "future result is " << result.get() << "\n";
}