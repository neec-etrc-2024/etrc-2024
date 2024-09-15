#ifndef RUNNABLE_HPP
#define RUNNABLE_HPP

// runメソッドを持つインターフェース
class IRunnable {
public:
  virtual void run() = 0;
  virtual ~IRunnable() {}
};

#endif // RUNNABLE_HPP