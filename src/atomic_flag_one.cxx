#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

std::atomic_flag condAtomicFlag{};

std::atomic<int> counter{};

#ifdef my_debug
constexpr int countLimit = 10;
#else
constexpr int countLimit = 1'000'000;
#endif

void ping() {
  while (counter <= countLimit) {
    condAtomicFlag.wait(true);
    condAtomicFlag.test_and_set();
#ifdef my_debug
    std::cerr << __FUNCTION__ << "" << '\n';
#endif

    ++counter;

    condAtomicFlag.notify_one();
  }
}

void pong() {
  while (counter <= countLimit) {
    condAtomicFlag.wait(false);
    condAtomicFlag.clear();
#ifdef my_debug
    std::cerr << __FUNCTION__ << "" << '\n';
#endif

    condAtomicFlag.notify_one();
  }
}

int main() {
  auto start = std::chrono::system_clock::now();

  //  condAtomicFlag.test_and_set();

  std::thread t1(ping);
  std::thread t2(pong);

  t1.join();
  t2.join();

  std::chrono::duration<double> duration =
      std::chrono::system_clock::now() - start;
  std::cout << "Duration:" << duration.count() << " seconds" << '\n';
}
