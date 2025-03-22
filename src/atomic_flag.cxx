#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

std::atomic_flag condAtomicFlag1{};
std::atomic_flag condAtomicFlag2{};

std::atomic<int> counter{};

#ifdef my_debug
constexpr int countLimit = 10;
#else
constexpr int countLimit = 1'000'000;
#endif

void ping() {
  while (counter <= countLimit) {
    condAtomicFlag1.wait(false);
    condAtomicFlag1.clear();
#ifdef my_debug
    std::cerr << __FUNCTION__ << "" << '\n';
#endif

    ++counter;

    condAtomicFlag2.test_and_set();
    condAtomicFlag2.notify_one();
  }
}

void pong() {
  while (counter <= countLimit) {
    condAtomicFlag2.wait(false);
    condAtomicFlag2.clear();
#ifdef my_debug
    std::cerr << __FUNCTION__ << "" << '\n';
#endif

    condAtomicFlag1.test_and_set();
    condAtomicFlag1.notify_one();
  }
}

int main() {
  auto start = std::chrono::system_clock::now();

  condAtomicFlag1.test_and_set();

  std::thread t1(ping);
  std::thread t2(pong);

  t1.join();
  t2.join();

  std::chrono::duration<double> duration =
      std::chrono::system_clock::now() - start;
  std::cout << "Duration:" << duration.count() << " seconds" << '\n';
}
