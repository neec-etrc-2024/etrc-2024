#ifndef _FRAME_BUFFER_HPP_
#define _FRAME_BUFFER_HPP_

#include <condition_variable>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <queue>
#include <stdio.h>

class FrameBuffer {
private:
  std::queue<cv::Mat> buffer;
  std::mutex mtx;
  std::condition_variable cv;
  const size_t maxSize = 10; // バッファの最大サイズ

public:
  void push(const cv::Mat &frame) {
    std::lock_guard<std::mutex> lock(mtx);
    if (buffer.size() >= maxSize) {
      buffer.pop(); // 古いフレームを捨てる
      fputs("Buffer overflow\n", stderr);
    }
    buffer.push(frame.clone()); // フレームをバッファにコピー
    cv.notify_one();
  }

  cv::Mat pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return !buffer.empty(); });
    cv::Mat frame = buffer.front();
    buffer.pop();
    return frame;
  }
};

#endif // _FRAME_BUFFER_HPP_