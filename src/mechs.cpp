#include "mechs.hpp"
#include "main.h"
#include "pros/motors.h"
#include "pros/motors.hpp"

extern pros::Motor left_intake_mtr;
extern pros::Motor right_intake_mtr;

extern pros::Motor flywheel_mtr;
extern pros::Motor flywheel_mtr_2;

extern pros::Motor left_lift_mtr;
extern pros::Motor right_lift_mtr;

extern pros::Motor lift_mtr;

extern pros::Motor conveyor_mtr;
extern pros::Motor conveyor_mtr_2;

extern pros::Motor intake_mtr;

extern pros::Motor slapper_mtr;

//pneumatics below
extern pros::ADIDigitalOut left_wall;
extern pros::ADIDigitalOut right_wall;

extern pros::ADIDigitalOut kicker;

extern pros::ADIDigitalOut grabber;


bool forward_intake_flag = false;
bool reverse_intake_flag = false;

bool flywheel_flag = false;

bool forward_conveyor_flag = false;
bool reverse_conveyor_flag = false;

bool slapper_flag = false;


// turn off motors; reset flags; retract pneumatics
void everything_off() {
  //intake
  left_intake_mtr.move(0);
  right_intake_mtr.move(0);

  intake_mtr.move(0);
  forward_intake_flag = false;
  reverse_intake_flag = false;

  //conveyor
  conveyor_mtr.move(0);
  conveyor_mtr_2.move(0);
  forward_conveyor_flag = false;
  reverse_conveyor_flag = false;

  //pneumatics
  left_wall.set_value(false);
  right_wall.set_value(false);
  grabber.set_value(false);

  //retract hang
  left_hang.set_value(false);
  right_hang.set_value(false);

  //flywheel
  // flywheel_mtr.move(0);
  // flywheel_mtr_2.move(0);
  // flywheel_flag = false;
}

// display wattage from mechs
void get_mech_watts() {
  pros::lcd::print(0, "Left intake: %f", left_intake_mtr.get_power());
  pros::lcd::print(1, "Right intake: %f", right_intake_mtr.get_power());
  pros::lcd::print(2, "Flywheel: %f", flywheel_mtr.get_power());
  pros::lcd::print(3, "Flywheel 2: %f", flywheel_mtr_2.get_power());
  pros::lcd::print(4, "Conveyor: %f", conveyor_mtr.get_power());
  pros::lcd::print(5, "Conveyor 2: %f", conveyor_mtr_2.get_power());
}



// run intake
void run_intake_forward() {
  // flag for intake so button can both turn on and off
  forward_intake_flag = not forward_intake_flag;
  reverse_intake_flag = false;

  if (forward_intake_flag) {
    // left_intake_mtr.move(127);
    // right_intake_mtr.move(127);
    intake_mtr.move(-127);
  } else {
    // left_intake_mtr.move(0);
    // right_intake_mtr.move(0);
    intake_mtr.move(0);
  }
  pros::delay(100);
}


// reverse intake
void run_intake_backward() {
  // flag for intake so button can both turn on and off
  reverse_intake_flag = not reverse_intake_flag;
  forward_intake_flag = false;

  if (reverse_intake_flag) {
    // left_intake_mtr.move(-127);
    // right_intake_mtr.move(-127);
    intake_mtr.move(127);
  } else {
    // left_intake_mtr.move(0);
    // right_intake_mtr.move(0);
    intake_mtr.move(0);
  }
  pros::delay(100);
}


// run flywheel
void run_flywheel(int power) {
  //flag for flywheel so button can both turn on and off
  flywheel_flag = not flywheel_flag;

  if (flywheel_flag) {
    flywheel_mtr.move(power);
    // flywheel_mtr_2.move(power);
  } else {
    flywheel_mtr.move(0);
    // flywheel_mtr_2.move(0);
  }
  pros::delay(250);
}

// Moves lift up or down
void move_lift(bool lift_state) {
  // experiment with position, should be the max position
  double position = (lift_state) ? 4500 : 0;
  left_lift_mtr.move_relative(position, 50);
  right_lift_mtr.move_relative(position, 50);
}


bool lift_up = false;

void move_lift_up() {
  if (not lift_up) {
    lift_mtr.set_brake_mode(MOTOR_BRAKE_COAST);
    lift_mtr.move_relative(-825, 100);
    lift_up = true;
  }
  pros::delay(250);
}

void move_lift_down() {
  if (lift_up) {
    lift_mtr.set_brake_mode(MOTOR_BRAKE_COAST);
    lift_mtr.move_relative(825, 100);
    lift_up = false;
  }
  pros::delay(250);
}

void move_lift_hang() {
  if (lift_up) {
    lift_mtr.set_brake_mode(MOTOR_BRAKE_HOLD);
    lift_mtr.move_relative(925, 100);
    lift_up = false;
  }
  pros::delay(250);
}

void move_lift_end() {
  lift_mtr.set_brake_mode(MOTOR_BRAKE_HOLD);
  lift_mtr.move_relative(-50, 100);
  pros::delay(250);
}

// Run conveyor
void run_conveyor_forward() {
  // flag for conveyor so button can both turn on and off
  forward_conveyor_flag = not forward_conveyor_flag;
  reverse_conveyor_flag = false;

  if (forward_conveyor_flag) {
    conveyor_mtr.move(127);
    conveyor_mtr_2.move(127);
  } else {
    conveyor_mtr.move(0);
    conveyor_mtr_2.move(0);
  }
  pros::delay(250);
}

//Run conveyor backwards
void run_conveyor_backward() {
  // flag for conveyor so button can both turn on and off
  reverse_conveyor_flag = not reverse_conveyor_flag;
  forward_conveyor_flag = false;

  if (reverse_conveyor_flag) {
    conveyor_mtr.move(-127);
    conveyor_mtr_2.move(-127);
  } else {
    conveyor_mtr.move(0);
    conveyor_mtr_2.move(0);
  }
  pros::delay(250);
}


// INTAKE FUNCTION
void intake() {
  left_intake_mtr.move(127);
  right_intake_mtr.move(127);
  forward_intake_flag = true;
  reverse_intake_flag = false;

  conveyor_mtr.move(0);
  conveyor_mtr_2.move(0);
  forward_conveyor_flag = false;
  reverse_conveyor_flag = false;
}

// OUTTAKE FUNCTION
void outtake() {
  left_intake_mtr.move(-127);
  right_intake_mtr.move(-127);
  forward_intake_flag = false;
  reverse_intake_flag = true;

  conveyor_mtr.move(-127);
  conveyor_mtr_2.move(-127);
  forward_conveyor_flag = false;
  reverse_conveyor_flag = false;
}

// INDEX FUNCTION
void index() {
  left_intake_mtr.move(127);
  right_intake_mtr.move(127);
  forward_intake_flag = true;
  reverse_intake_flag = false;

  conveyor_mtr.move(127);
  conveyor_mtr_2.move(127);
  forward_conveyor_flag = true;
  reverse_conveyor_flag = false;
}

void slapper()
{
  if (slapper_flag == false)
  {
    slapper_mtr.set_brake_mode(MOTOR_BRAKE_COAST);
    slapper_mtr.move(127); 
    slapper_flag = true;
  }
  else
  {
    slapper_mtr.move(0);
    slapper_flag = false;
  }
}