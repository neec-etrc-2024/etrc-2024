#ifndef _SELF_LOCALIZATION_HPP_
#define _SELF_LOCALIZATION_HPP_

namespace informations {
struct Location {
  double x;
  double y;
  double theta;
};

class SelfLocalization {
private:
  /* data */
public:
  SelfLocalization(/* args */);
  void get_location(Location &location) const;
  void update();
  ~SelfLocalization();
};

inline SelfLocalization::SelfLocalization(/* args */) {}

inline SelfLocalization::~SelfLocalization() {}

inline void SelfLocalization::get_location(Location &location) const {
  location.x = 0;
  location.y = 0;
  location.theta = 0;
}

inline void SelfLocalization::update() {}

} // namespace informations

#endif // _SELF_LOCALIZATION_HPP_