#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

std::atomic<int> counter{};

#ifdef my_debug
constexpr int countLimit = 10;
#else
constexpr int countLimit = 1'000'000;
#endif

void ping() {
  while (counter <= countLimit) {

    ++counter;
    __asm__ __volatile__("");
  }
}

int main() {
  auto start = std::chrono::system_clock::now();

  ping();

  std::chrono::duration<double> duration =
      std::chrono::system_clock::now() - start;
  std::cout << "Duration:" << duration.count() << " seconds" << '\n';
}
