#include <functional>
#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <memory>
#include <string>

class Widget {
public:
  void setPtr(std::unique_ptr<std::string>&& ptr) {
    p = std::move(ptr);
  }
private:
  std::unique_ptr<std::string> p;
};

int main() {
  Widget w;
  std::unique_ptr<std::string> ptr = std::make_unique<std::string>("C++");
  w.setPtr(std::move(ptr));
}