#include "mechs.hpp"
#include "main.h"
#include "pros/motors.hpp"

extern pros::Motor left_intake_mtr;
extern pros::Motor right_intake_mtr;

extern pros::Motor flywheel_mtr;
extern pros::Motor flywheel_mtr_2;


bool forward_intake_flag = false;
bool reverse_intake_flag = false;

bool flywheel_flag = false;


// turn off all motors; reset all flags
void everything_off() {
  left_intake_mtr.move(0);
  right_intake_mtr.move(0);
  flywheel_mtr.move(0);
  flywheel_mtr_2.move(0);

  forward_intake_flag = false;
  reverse_intake_flag = false;
  flywheel_flag = false;
}

// display wattage from mechs
void get_mech_watts() {
  pros::lcd::print(0, "Left intake: %f", left_intake_mtr.get_power());
  pros::lcd::print(1, "Right intake: %f", right_intake_mtr.get_power());
  pros::lcd::print(2, "Flywheel: %f", flywheel_mtr.get_power());
}



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


// run flywheel
void run_flywheel() {
  //flag for flywheel so button can both turn on and off
  flywheel_flag = not flywheel_flag;

  if (flywheel_flag) {
    flywheel_mtr.move(127);
    flywheel_mtr_2.move(127);
  } else {
    flywheel_mtr.move(0);
    flywheel_mtr_2.move(0);
  }
  pros::delay(250);
}