#include "Scenario.hpp"

#include <stdio.h>

bool Scenario::run() {
  if (!enable) { // enableがfalseの場合は何せず即終了
    return true;
  }
  if (!init) {
    pup_motor_reset_count(left);
    pup_motor_reset_count(right);
    line_monitor->set_trace_left(edge == LEFT);
    init = true;

    fputs("SCENARIO : ", stdout);
    fputs(name.c_str(), stdout);
    fputc('\n', stdout);
    int left = pup_motor_get_count(this->left);
    int right = pup_motor_get_count(this->right);
    printf("left: %d, right: %d\n", left, right);
  }

  pid_profile.run(line_monitor->get_differences());

  bool result = false;

  switch (mode) {
  case DISTANCE: {
    int left_count = pup_motor_get_count(this->left);
    int right_count = pup_motor_get_count(this->right);
    result = left_count >= end && right_count >= end;
    if (debug) {
      printf("%s target=%d left=%d right=%d\n", name.c_str(), end, left_count,
             right_count);
    }

    break;
  }
  case BLUE: {
    int blue_count = line_monitor->get_blue_count();
    if (check_mode == OVER) {
      printf("%s mode=OVER target=%d blue=%d\n", name.c_str(), end, blue_count);
      result = blue_count >= end;
    } else {
      printf("%s mode=UNDER target=%d blue=%d\n", name.c_str(), end,
             blue_count);
      result = blue_count < end;
    }
    break;
  }
  }

  return result;
}