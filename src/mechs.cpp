#include "mechs.hpp"
#include "main.h"

extern pros::Motor left_intake_mtr;
extern pros::Motor right_intake_mtr;

bool forward_intake_flag = false;
bool reverse_intake_flag = false;

// run intake
void run_intake_forward() {
  // flag for intake so button can both turn on and off
  forward_intake_flag = not forward_intake_flag;
  reverse_intake_flag = false;

  if (forward_intake_flag) {
    left_intake_mtr.move(127);
    right_intake_mtr.move(127);
  } else {
    left_intake_mtr.move(0);
    right_intake_mtr.move(0);
  }
  pros::delay(250);
}


// reverse intake
void run_intake_backward() {
  // flag for intake so button can both turn on and off
  reverse_intake_flag = not reverse_intake_flag;
  forward_intake_flag = false;

  if (reverse_intake_flag) {
    left_intake_mtr.move(-127);
    right_intake_mtr.move(-127);
  } else {
    left_intake_mtr.move(0);
    right_intake_mtr.move(0);
  }
  pros::delay(250);
}