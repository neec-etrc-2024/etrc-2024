#ifndef _PERIODIC_EXECUTOR_HPP
#define _PERIODIC_EXECUTOR_HPP

#include "IRunnable.hpp"
#include <atomic>
#include <chrono>
#include <thread>

typedef unsigned long interval_t;

// 周期タスクを実行するクラス
class PeriodicExecutor {
private:
  IRunnable *task;              // 実行するタスク
  unsigned long interval;       // 実行間隔 (ミリ秒)
  unsigned long last_exec_time; // 最後に実行した時間
  std::atomic<bool> running;    // 実行中かどうか Falseで停止
  std::thread executor_thread;  // 実行スレッド

public:
  PeriodicExecutor(IRunnable *task, interval_t interval)
      : task(task), interval(interval), last_exec_time(0), running(false) {}

  void start() {
    running.store(true);
    executor_thread = std::thread(&PeriodicExecutor::run, this);
  };
  void stop() { running.store(false); };
  void join() { executor_thread.join(); };
  void run() {
    while (running.load()) {
      task->run();
      unsigned long current_time =
          std::chrono::high_resolution_clock::now().time_since_epoch().count();
      unsigned long sleep_time = interval - current_time - last_exec_time;
      last_exec_time = current_time;
      if (sleep_time > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
      }
    }
  };
};

#endif // _PERIODIC_EXECUTOR_HPP