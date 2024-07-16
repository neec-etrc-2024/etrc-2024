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

SelfLocalization::SelfLocalization(/* args */) {}

SelfLocalization::~SelfLocalization() {}

} // namespace informations

#endif // _SELF_LOCALIZATION_HPP_