#include "informations/CameraLineMonitor.hpp"
#include "FrameBuffer.hpp"
#include "JsonParser.hpp"

bool sortByYAscXAsc(const cv::Point &a, const cv::Point &b) {
  return (a.y < b.y) || (a.y == b.y && a.x < b.x);
}

bool sortByXAscYAsc(const cv::Point &a, const cv::Point &b) {
  return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

bool sortByXAscYDesc(const cv::Point &a, const cv::Point &b) {
  return (a.x < b.x) || (a.x == b.x && a.y > b.y);
}

bool sortByXDescYAsc(const cv::Point &a, const cv::Point &b) {
  return (a.x > b.x) || (a.x == b.x && a.y < b.y);
}

bool sortByXDescYDesc(const cv::Point &a, const cv::Point &b) {
  return (a.x > b.x) || (a.x == b.x && a.y > b.y);
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

bool comparePointsY(const cv::Point2f &a, const cv::Point2f &b) {
  return a.y < b.y;
}

extern WindowManager wm;

int g_roi_w;
// double epsilon = config["epsilon"];

void informations::CameraLineMonitor::update(cv::Mat &img, FrameBuffer *buf) {
  int roi_x = 520;
  int roi_y = 1032;
  int roi_w = 600;
  int roi_h = 200;

  FILE *fp = fopen("camera.conf", "r");
  if (fp != NULL) {
    fscanf(fp, "%d %d %d %d", &roi_x, &roi_y, &roi_w, &roi_h);
    fclose(fp);
  }
  g_roi_w = roi_w;

  cv::Mat roi = img(cv::Rect(roi_x, roi_y, roi_w, roi_h));
  cv::Mat hsv;
  cv::cvtColor(roi, hsv, cv::COLOR_BGR2HSV);
  cv::Scalar lower(0, 0, 0);
  cv::Scalar upper(180, 255, 100);
  cv::Scalar b_lower(100, 150, 0);
  cv::Scalar b_upper(140, 255, 255);
  cv::Mat mask;
  cv::inRange(hsv, lower, upper, mask);

  cv::Mat b_mask;
  cv::inRange(hsv, b_lower, b_upper, b_mask);

  int blue = cv::countNonZero(b_mask);
  blue_count.store(blue);

  mask = mask | b_mask;

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

  // printf("max_area: %f\n", max_area);
  int roi_center = roi_w / 2;
  // 重心のX座標が最も真ん中に近い輪郭を抽出
  std::sort(contours.begin(), contours.end(),
            [roi_center](const std::vector<cv::Point> &a,
                         const std::vector<cv::Point> &b) {
              cv::Moments ma = cv::moments(a);
              cv::Moments mb = cv::moments(b);
              double center_a_x = ma.m10 / ma.m00;
              double center_b_x = mb.m10 / mb.m00;
              return std::abs(center_a_x - roi_center) <
                     std::abs(center_b_x - roi_center);
            });

  int idx = 0;
  for (size_t i = 0; i < contours.size(); i++) {
    if (cv::contourArea(contours[i]) > 20000) {
      idx = i;
      break;
    }
  }
  if (cv::contourArea(contours[0]) <= 20000) {
    // printf("Deka Countras Not Found\n");
    return;
  }

  std::vector<cv::Point> max_contour = contours[idx];

  std::sort(max_contour.begin(), max_contour.end(), sortByXAscYAsc);
  cv::Point top_left = max_contour[0];
  std::sort(max_contour.begin(), max_contour.end(), sortByXDescYAsc);
  cv::Point top_right = max_contour[0];
  std::sort(max_contour.begin(), max_contour.end(), sortByXAscYDesc);
  cv::Point bottom_left = max_contour[0];
  std::sort(max_contour.begin(), max_contour.end(), sortByXDescYDesc);
  cv::Point bottom_right = max_contour[0];

  // 輪郭の近似
  std::vector<cv::Point> approx;
  cv::approxPolyDP(max_contour, approx, 0.1 * cv::arcLength(max_contour, true),
                   true);

  // 輪郭のX最小最大、Y最小最大を求める
  int min_x = roi_w;
  int max_x = 0;
  int min_y = roi_h;
  int max_y = 0;

  for (size_t i = 0; i < approx.size(); i++) {
    if (approx[i].x < min_x) {
      min_x = approx[i].x;
    }
    if (approx[i].x > max_x) {
      max_x = approx[i].x;
    }
    if (approx[i].y < min_y) {
      min_y = approx[i].y;
    }
    if (approx[i].y > max_y) {
      max_y = approx[i].y;
    }
  }

  top_left = cv::Point(min_x, min_y);
  top_right = cv::Point(max_x, min_y);
  bottom_left = cv::Point(min_x, max_y);
  bottom_right = cv::Point(max_x, max_y);

  // 描画
  // cv::drawContours(roi, std::vector<std::vector<cv::Point>>{approx}, -1,
  //               cv::Scalar(255, 255, 0), 2);

  // 凸包
  std::vector<cv::Point> hull;
  cv::convexHull(max_contour, hull);

  // 最小外接矩形
  cv::RotatedRect rect = cv::minAreaRect(hull);

  cv::Point2f vertices[4];
  rect.points(vertices);

  // Y昇順でソート
  std::sort(vertices, vertices + 4, comparePointsY);
  if (vertices[0].x > vertices[1].x) {
    std::swap(vertices[0], vertices[1]);
  }
  if (vertices[2].x < vertices[3].x) {
    std::swap(vertices[2], vertices[3]);
  }

  top_left = vertices[0];
  top_right = vertices[1];
  bottom_right = vertices[2];
  bottom_left = vertices[3];

  int diff;
  // trace_left = false;
  if (trace_left.load()) {
    printf("trace_left\n");
    diff = (top_left.x + bottom_left.x) / 2 - roi_w / 2;
  } else {
    printf("trace_right\n");
    diff = (top_right.x + bottom_right.x) / 2 - roi_w / 2;
  }

  line_width.store(top_right.x - top_left.x);

  if (buf != nullptr) {
    // top、bottomを囲む
    cv::line(roi, top_left, top_right, cv::Scalar(0, 0, 255), 2);
    cv::line(roi, bottom_left, bottom_right, cv::Scalar(0, 0, 255), 2);
    // left、rightを囲む
    cv::line(roi, top_left, bottom_left, cv::Scalar(0, 0, 255), 2);
    cv::line(roi, top_right, bottom_right, cv::Scalar(0, 0, 255), 2);
    // x軸の中心線
    cv::line(roi, cv::Point(roi_w / 2, 0), cv::Point(roi_w / 2, roi_h),
             cv::Scalar(0, 0, 255), 2);
    // diffの線
    cv::line(roi, cv::Point(roi_w / 2 + diff, 0),
             cv::Point(roi_w / 2 + diff, roi_h), cv::Scalar(0, 255, 0), 2);

    // max_contourの描画
    /*
    for (size_t i = 0; i < max_contour.size(); i++) {
      cv::circle(roi, max_contour[i], 3, cv::Scalar(255, 0, 0), -1);
    }
    */

    buf->push(roi);
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
  this->diff = diff / (roi_w / 2.0);
  mtx.unlock();

  // printf("diff: %f\n", this->diff);
}