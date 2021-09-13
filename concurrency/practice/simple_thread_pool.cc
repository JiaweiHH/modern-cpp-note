#include <thread>
#include <iostream>
#include <future>
#include <queue>

class ThreadPool {
public:

private:
  std::queue<std::thread> q;
};