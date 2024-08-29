#include "gui/Window.hpp"

void Window::create_window() { // こいつをguiコンテキストで呼ぶ
  window = new Fl_Window(w, h);
  window->label(title.c_str());
  box = new Fl_Box(0, 0, w, h);
  fl_data = new uchar[w * h * 3];
  memset(fl_data, 0, w * h * 3);
  fl_img = new Fl_RGB_Image(fl_data, w, h, 3);

  window->end();
  window->show();
  box->image(fl_img);
}

void Window::set_frame(cv::Mat &frame) {
  std::lock_guard<std::mutex> lock(mtx);
  frame.copyTo(this->frame);
  cv::resize(this->frame, this->frame, cv::Size(w, h));
}

void Window::update() { // こいつをguiコンテキストで呼ぶ
  mtx.lock();
  if (frame.empty()) {
    mtx.unlock();
    return;
  }
  cv::Mat rgb;
  cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
  mtx.unlock();
  memcpy(fl_data, rgb.data, w * h * 3);
  fl_img->uncache();
  box->redraw();
}