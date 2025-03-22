#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

std::atomic<bool> atomicBool{};

std::atomic<int> counter{};

#ifdef my_debug
constexpr int countLimit = 10;
#else
constexpr int countLimit = 1'000'000;
#endif

void ping() {
  while (counter <= countLimit) {
    atomicBool.wait(true);
    atomicBool.store(true);
#ifdef my_debug
    std::cerr << __FUNCTION__ << "" << '\n';
#endif

    ++counter;

    atomicBool.notify_one();
  }
}

void pong() {
  while (counter <= countLimit) {
    atomicBool.wait(false);
    atomicBool.store(false);
#ifdef my_debug
    std::cerr << __FUNCTION__ << "" << '\n';
#endif

    ++counter;

    atomicBool.notify_one();
  }
}

int main() {
  std::cout << std::boolalpha << '\n';
  std::cout << "atomicBool.is_lock_free():" << atomicBool.is_lock_free()
            << '\n';

  std::cout << '\n';
  auto start = std::chrono::system_clock::now();

  atomicBool.store(false);

  std::thread t1(ping);
  std::thread t2(pong);

  t1.join();
  t2.join();

  std::chrono::duration<double> duration =
      std::chrono::system_clock::now() - start;
  std::cout << "Duration:" << duration.count() << " seconds" << '\n';
}
