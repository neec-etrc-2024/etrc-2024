#include "informations/CameraLineMonitor.hpp"

bool sortByYAscXAsc(const cv::Point &a, const cv::Point &b) {
  return (a.y < b.y) || (a.y == b.y && a.x < b.x);
}

bool sortByYAscXDesc(const cv::Point &a, const cv::Point &b) {
  return (a.y < b.y) || (a.y == b.y && a.x > b.x);
}

bool sortByYDescXAsc(const cv::Point &a, const cv::Point &b) {
  return (a.y > b.y) || (a.y == b.y && a.x < b.x);
}

bool sortByYDescXDesc(const cv::Point &a, const cv::Point &b) {
  return (a.y > b.y) || (a.y == b.y && a.x > b.x);
}

void informations::CameraLineMonitor::update(cv::Mat &img) {
  int roi_x = 520;
  int roi_y = 500;
  int roi_w = 600;
  int roi_h = 400;

  cv::Mat roi = img(cv::Rect(roi_x, roi_y, roi_w, roi_h));
  cv::Mat gray;
  cv::cvtColor(roi, gray, cv::COLOR_BGR2GRAY);
  cv::Mat bin;
  cv::threshold(gray, bin, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
  cv::bitwise_not(bin, bin);

  // ノイズ除去
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
  cv::morphologyEx(bin, bin, cv::MORPH_OPEN, kernel);

  // 輪郭抽出
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(bin, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  // 面積最大の輪郭を抽出
  double max_area = 0;
  int max_area_idx = 0;

  for (int i = 0; i < contours.size(); i++) {
    double area = cv::contourArea(contours[i]);
    if (area > max_area) {
      max_area = area;
      max_area_idx = i;
    }
  }

  std::vector<cv::Point> max_contour = contours[max_area_idx];
  std::sort(max_contour.begin(), max_contour.end(), sortByYAscXAsc);
  cv::Point top_left = max_contour[0];
  std::sort(max_contour.begin(), max_contour.end(), sortByYAscXDesc);
  cv::Point top_right = max_contour[0];
  std::sort(max_contour.begin(), max_contour.end(), sortByYDescXAsc);
  cv::Point bottom_left = max_contour[0];
  std::sort(max_contour.begin(), max_contour.end(), sortByYDescXDesc);
  cv::Point bottom_right = max_contour[0];

  int diff;
  if (trace_left) {
    diff = (top_left.x + bottom_left.x) / 2 - 300;
  } else {
    diff = (top_right.x + bottom_right.x) / 2 - 300;
  }

  // 300+-100の範囲に圧縮

  if (diff > 100) {
    diff = 100;
  } else if (diff < -100) {
    diff = -100;
  }

  // printf("diff: %d\n", diff);

  mtx.lock();
  this->diff = diff / 100.0;
  mtx.unlock();

  // printf("diff: %f\n", this->diff);
}