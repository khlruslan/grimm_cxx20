#include <atomic>
#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>

std::counting_semaphore<1> signal2Ping(0);
std::counting_semaphore<1> signal2Pong(0);

std::atomic<int> counter{};

#ifdef my_debug
constexpr int countLimit = 10;
#else
constexpr int countLimit = 1'000'000;
#endif

void ping() {
  while (counter <= countLimit) {
    signal2Ping.acquire();
#ifdef my_debug
    std::cerr << __FUNCTION__ << "" << '\n';
#endif

    ++counter;

    signal2Pong.release();
  }
}

void pong() {
  while (counter <= countLimit) {
    signal2Pong.acquire();
#ifdef my_debug
    std::cerr << __FUNCTION__ << "" << '\n';
#endif
    signal2Ping.release();
  }
}

int main() {
  auto start = std::chrono::system_clock::now();

  signal2Ping.release();

  std::thread t1(ping);
  std::thread t2(pong);

  t1.join();
  t2.join();

  std::chrono::duration<double> duration =
      std::chrono::system_clock::now() - start;
  std::cout << "Duration:" << duration.count() << " seconds" << '\n';
}
