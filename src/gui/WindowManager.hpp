#pragma once

#include "gui/Window.hpp"
#include <Fl/Fl.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_RGB_Image.H>
#include <Fl/Fl_Window.H>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <vector>

class WindowManager {
private:
  std::vector<Window *> windows;
  volatile bool is_ready;

public:
  WindowManager() : windows(), is_ready(false) {}

  void main_th(int w, int h);
  int create_window(int w, int h, std::string title = "window");
  void update_window(int id, cv::Mat &frame);
  void destroy_window(int id);
  bool ready() { return is_ready; }

  static void awake_update(void *window);
  static void awake_create(void *window);
  static void awake_destroy(void *window);
};