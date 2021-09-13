/**
 * @brief C++11 为原子操作定义了六种不同的内存顺序选项
 * 1. relax: 指定为这个的所有原子操作保证顺序性
 * 2. release: 一般用于写操作，保证前面的 rw 操作不会被重排到 release 语句之后
 *    （配合 acquire 和 consumer 使用的时候，对于使用 acquire 或 consumer 的线程有不同的含义）
 * 3. acquire: 一般用于读操作，保证后续的 rw 操作不会被重排到 acquire 之前
 *    （配合 release 使用的语义见下面的例子注释）
 * 4. consumer: 特殊的 acquire，只保证后续有依赖关系的变量操作不会被重排到它之前
 *    （配合 release 使用的语义见下面的例子注释）
 * 5. acq_rel: 结合 release 和 acquire，唯一确定了内存屏障；如果 acq_rel 作用在原子变量 M 上
 *    其他线程中所有对 M 的 release 及其之前的写入都对当前线程从 acq_rel 开始之后可见
 *    截止到 acq_rel 的所有内存写入都对另外一个线程对 M 的 acquire 可见
 * 6. seq_cst: 保证线程间所有变量操作的顺序一致性，宏观上看相当于单线程在执行
 * 
 * https://blog.csdn.net/wxj1992/article/details/103656486?spm=1001.2014.3001.5501
 */

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

/**
 * @brief 宽松模型
 * 1. 单个线程内的操作都是顺序执行的，不允许指令重排
 * 2. 不同线程间原子操作的顺序是任意的
 */
void relax_model() {
  std::atomic<int> counter{0};
  std::vector<std::thread> vt;
  for(int i = 0; i < 100; ++i) {
    vt.emplace_back([&] {
      counter.fetch_add(1, std::memory_order_relaxed);
    });
  }
  
  for(auto& t : vt)
    t.join();
  std::cout << "current counter: " << counter << "\n";
}

/**
 * @brief 释放/消费 模型
 * 1. 线程 A 中对原子变量 M 的 release 操作确保 release 之前的内存读写都不能重新排到该指令之后
 * 2. 截止到线程 A 中所有 M 所依赖的内存写入都对另外线程对 M 的 consumer 以及之后的操作可见
 * 3. 线程 B 中对原子变量 M 的 consumer 操作确保该操作之后，依赖该 consumer 操作的 load, store 不能被重排到它之前
 */
void release_consume_mode() {
  std::atomic<int*> ptr{nullptr};
  int v;

  std::thread producer([&] {
    int *p = new int(42);
    v = 1024;
    /* 保证之前的所有读写操作不会被放到该指令之后 */
    ptr.store(p, std::memory_order_release);
  });

  std::thread consumer([&] {
    int *p;
    /* 保证之后对 ptr 有依赖的读写操作不会被放到该语句之前 */
    while(!(p = ptr.load(std::memory_order_consume)));
    std::cout << "p: " << *p << "\n";
    std::cout << "v: " << v << "\n";
  });
  producer.join();
  consumer.join();
}

/**
 * @brief 释放/获取 模型
 * 1. 线程 A 中对原子变量 M 的 release 操作确保 release 之前的内存读写都不能重新排到该指令之后
 * 2. 截止到线程 A 该 release 操作的所有内存写入对于其他线程对 M 的 acquire 开始之后的指令可见
 * 3. 线程 B 中 acquire 之后的 load, store 操作不能被重排到它之前
 */
void release_acquire_model() {
  std::vector<int> v;
  std::atomic<int> flag = {0};

  std::thread release([&]() {
    v.push_back(42);
    /* 前置屏障 */
    flag.store(1, std::memory_order_release);
  });

  std::thread acqrel([&]() {
    int expected = 1; /* 该语句必定在 compare_exchange_strong 之前执行 */
    while(!flag.compare_exchange_strong(expected, 2, std::memory_order_acq_rel))
      expected = 1;   /* 该语句必定在 compare_exchange_strong 之后执行 */
    
    // flag 变为 2
  });

  std::thread acquire([&]() {
    while (flag.load(std::memory_order_acquire) < 2);
    std::cout << v.at(0) << std::endl;  /* 必定是 42 */
  });

  release.join();
  acqrel.join();
  acquire.join();
}

/**
 * @brief 顺序一致性模型
 * 所有的原子操作看上去就和一个线程顺序执行一样
 */
void seq_cst_model() {
  std::atomic<int> counter(0);
  std::vector<std::thread> vt;
  for(int i = 0; i < 100; ++i) {
    vt.emplace_back([&]{
      counter.fetch_add(1, std::memory_order_seq_cst);
    });
  }

  for(auto& t : vt)
    t.join();
}