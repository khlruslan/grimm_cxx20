#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

bool dataReady{false};

std::mutex mutex_;
std::condition_variable condVar1;
std::condition_variable condVar2;

std::atomic<int> counter{};

#ifdef my_debug
constexpr int countLimit = 10;
#else
constexpr int countLimit = 1'000'000;
#endif

void ping() {
  while (counter <= countLimit) {
    {
      std::unique_lock<std::mutex> lck(mutex_);
      condVar1.wait(lck, [] { return dataReady == false; });
      dataReady = true;
#ifdef my_debug
      std::cerr << __FUNCTION__ << "" << '\n';
#endif
    }
    ++counter;
    condVar2.notify_one();
  }
}

void pong() {
  while (counter <= countLimit) {
    {
      std::unique_lock<std::mutex> lck(mutex_);
      condVar2.wait(lck, [] { return dataReady == true; });
      dataReady = false;
#ifdef my_debug
      std::cerr << __FUNCTION__ << "" << '\n';
#endif
    }
    condVar1.notify_one();
  }
}

int main() {
  auto start = std::chrono::system_clock::now();

  std::thread t1(ping);
  std::thread t2(pong);

  t1.join();
  t2.join();

  std::chrono::duration<double> duration =
      std::chrono::system_clock::now() - start;
  std::cout << "Duration:" << duration.count() << " seconds" << '\n';
}
