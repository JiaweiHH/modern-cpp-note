#include <iostream>
#include <thread>
#include <chrono>

int v = 1;

/**
 * @brief std::mutex 是操作系统级的功能
 * 
 * 保障在互斥锁操作期间，所操作变量的内存与临界区外进行隔离，
 * 这是一个非常强的同步条件
 */

/**
 * @brief std::lock_guard 可以在栈上对象被销毁的时候自动释放锁
 * std::lock_guard 不能显式调用 lock 和 unlock
 */
void critical_section_use_lock_guard(int change_v) {
  static std::mutex mtx;
  std::lock_guard<std::mutex> lock(mtx);
  v = change_v; // 临界区操作

  // 离开临界区，mtx 被自动释放
}

/**
 * @brief std::unique_lock 的对象会独占的方式管理 mutex 对象的 lock 和 unlock
 * std::unique_lock 可以在声明后的任意位置调用 lock 和 unlock，以此缩小锁的范围
 * 
 * 另外当用到了条件变量 std::condition_variable::wait 的时候，必须使用 unique_lock
 */
void critical_section_use_unique_lock(int change_v) {
  static std::mutex mtx_1;
  
  /* 第一组临界区 */
  std::unique_lock<std::mutex> lock(mtx_1);
  v = change_v;
  std::cout << "thread id: " << std::this_thread::get_id() << ", v = " << v << "\n";
  lock.unlock();

  /* 在此期间任何人都可以抢夺 v 的持有权 */
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1000ms);

  /* 第二组临界区 */
  lock.lock();
  v += 1;
  std::cout << "thread id: " << std::this_thread::get_id() << ", v = " << v << "\n";

  // unique_lock 栈上对象被释放，mutex 自动解锁
}

int main() {
  /* lock_guard 的使用 */
  std::cout << "lock_guard: \n";
  std::thread t1(critical_section_use_lock_guard, 2), t2(critical_section_use_lock_guard, 3);
  t1.join();
  t2.join();
  std::cout << v << "\n";

  /* unique_lock 的使用 */
  std::cout << "unique_lock: \n";
  std::thread t3(critical_section_use_unique_lock, 2), t4(critical_section_use_unique_lock, 3);
  t3.join();
  t4.join();
  std::cout << v << "\n";
}