#include "gui/WindowManager.hpp"

void WindowManager::main_th(int w, int h) {
  Fl::lock();
  Window *window = new Window(w, h, "main");
  windows.push_back(window);
  window->create_window();
  is_ready = true;
  printf("TRUE\n");
  Fl::run();
  Fl::unlock();
}

int WindowManager::create_window(int w, int h, std::string title) {
  int id = windows.size();
  Window *window = new Window(w, h, title);
  windows.push_back(window);
  Fl::awake(awake_create, window);
  return id;
}

void WindowManager::update_window(int id, cv::Mat &frame) {
  if (id < 0 || id >= windows.size()) {
    return;
  }
  windows[id]->set_frame(frame);
  Fl::awake(awake_update, windows[id]);
}

void WindowManager::destroy_window(int id) {
  if (id < 0 || id >= windows.size()) {
    return;
  }
  Fl::awake(awake_destroy, windows[id]);
}

void WindowManager::awake_update(void *window) {
  Window *w = (Window *)window;
  w->update();
}

void WindowManager::awake_create(void *window) {
  Window *w = (Window *)window;
  w->create_window();
}

void WindowManager::awake_destroy(void *window) { return; }