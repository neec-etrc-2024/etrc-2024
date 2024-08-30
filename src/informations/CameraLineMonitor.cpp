#include "informations/CameraLineMonitor.hpp"
#include "gui/WindowManager.hpp"

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

extern WindowManager wm;

void informations::CameraLineMonitor::update(cv::Mat &img, int window_id) {
  int roi_x = 520;
  int roi_y = 1032;
  int roi_w = 600;
  int roi_h = 200;

  FILE *fp = fopen("camera.conf", "r");
  if (fp != NULL) {
    fscanf(fp, "%d %d %d %d", &roi_x, &roi_y, &roi_w, &roi_h);
    fclose(fp);
  }

  cv::Mat roi = img(cv::Rect(roi_x, roi_y, roi_w, roi_h));
  cv::Mat hsv;
  cv::cvtColor(roi, hsv, cv::COLOR_BGR2HSV);
  cv::Scalar lower(0, 0, 0);
  cv::Scalar upper(180, 255, 100);
  cv::Mat mask;
  cv::inRange(hsv, lower, upper, mask);

  // ノイズ除去
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
  cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

  // 輪郭抽出
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  if (contours.size() == 0) {
    printf("NO CONTOURS\n");
    return;
  }

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
  // trace_left = false;
  if (trace_left.load()) {
    diff = (top_left.x + bottom_left.x) / 2 - 300;
  } else {
    diff = (top_right.x + bottom_right.x) / 2 - 300;
  }

  line_width.store(top_right.x - top_left.x);

  if (window_id >= 0) {
    // x軸の中心線
    cv::line(roi, cv::Point(300, 0), cv::Point(300, 400), cv::Scalar(0, 0, 255),
             2);
    // diffの線
    cv::line(roi, cv::Point(300 + diff, 0), cv::Point(300 + diff, 400),
             cv::Scalar(0, 255, 0), 2);

    wm.update_window(window_id, roi);
  }

  // 300+-100の範囲に圧縮

  /*
    if (diff > 100) {
      diff = 100;
    } else if (diff < -100) {
      diff = -100;
    }
  */
  // printf("diff: %d\n", diff);

  mtx.lock();
  this->diff = diff / 300.0;
  mtx.unlock();

  // printf("diff: %f\n", this->diff);
}