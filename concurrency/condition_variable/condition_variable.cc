#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

/* 生产者——消费者 例子 */
int main() {
  std::queue<int> produced_nums;
  std::mutex mtx;
  std::condition_variable cv;
  bool notified = false;

  /* 生产者 */
  auto producer = [&] {
    for(int i = 0; ; ++i) {
      std::this_thread::sleep_for(std::chrono::milliseconds(900));
      std::unique_lock<std::mutex> lock(mtx);
      std::cout << "producing " << i << "\n";
      produced_nums.push(i);
      notified = true;
      cv.notify_all();
    }
  };

  /* 消费者 */
  auto consumer = [&] {
    while(true) {
      std::unique_lock<std::mutex> lock(mtx);

      /**
       * @brief 等待条件变量
       * 
       * 关于为什么使用 while，是因为存在虚假唤醒
       * 1. 虚假唤醒是指，在一个线程被唤醒并加锁之前，另一个线程插进来完成了更改条件变量等一系列操作，导致该线程被虚假唤醒
       * 2. 又或者是因为被中断或信号打断唤醒了
       * 
       * 另外关于 cv.wait 函数
       * 1. 当前线程调用 wait 会被阻塞
       * 2. 当线程被阻塞时，该函数会调用 unlock() 释放锁，使得其他被阻塞在锁竞争上的线程得以继续执行
       * 3. 一旦 notify()/notify_all() 唤醒了当前线程，wait 内部会自动执行 lock()
       */
      while(!notified)
        cv.wait(lock);
      
      lock.unlock();  // 短暂的取消锁，使得生成者有机会在消费者消费之前继续生产
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      lock.lock();
      while(!produced_nums.empty()) {
        std::cout << "consuming " << produced_nums.front() << "\n";
        produced_nums.pop();
      }
      notified = false;
    }
  };

  std::thread p(producer);
  std::thread cs[2];
  for(int i = 0; i < 2; ++i) {
    cs[i] = std::thread(consumer);
  }
  p.join();
  for(int i = 0; i < 2; ++i) {
    cs[i].join();
  }
}