#pragma once

#include <Fl/Fl.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_RGB_Image.H>
#include <Fl/Fl_Window.H>
#include <mutex>
#include <opencv2/opencv.hpp>

class Window {
private:
  int w, h;
  std::string title;
  Fl_Window *window;
  Fl_Box *box;
  Fl_RGB_Image *fl_img;
  uchar *fl_data;
  cv::Mat frame;
  std::mutex mtx;

public:
  Window(int w, int h, std::string title = "window")
      : w(w), h(h), title(title), window(), box(), fl_img(), fl_data(), frame(),
        mtx() {}

  void create_window();
  void set_frame(cv::Mat &frame);
  void update();
};