#include "drive.hpp"
#include "main.h"
#include "pros/motors.h"

extern pros::Motor front_left_mtr;
extern pros::Motor front_right_mtr;
extern pros::Motor back_left_mtr;
extern pros::Motor back_right_mtr;

bool brake_flag = false;

void tank_control(int left, int right) {

    // deadzones
    if (abs(left) < 10) {
      left = 0;
    }

    if (abs(right) < 10) {
      right = 0;
    }

    // brake logic
    if (left == 0) {
      left_brake();
    }

    if (right == 0) {
      right_brake();
    }
    brake_flag = false;

    // actually runs the motors
    front_left_mtr.move(left);
    back_left_mtr.move(left);
    back_right_mtr.move(right);
    front_right_mtr.move(right);
}

//display wattage from base
void get_base_watts() {
    pros::lcd::print(0, "front left: %f", front_left_mtr.get_power());
    pros::lcd::print(1, "front right: %f", front_right_mtr.get_power());
    pros::lcd::print(2, "back left: %f", back_left_mtr.get_power());
    pros::lcd::print(3, "back right: %f", back_right_mtr.get_power());
}

//braking
void brake() {
  front_left_mtr.brake();
  back_left_mtr.brake();
  front_right_mtr.brake();
  back_right_mtr.brake();
}

void right_brake() {
  front_right_mtr.brake();
  back_right_mtr.brake();
}

void left_brake() {
  front_left_mtr.brake();
  back_left_mtr.brake();
}


// setting brake modes
void brake_hold() {
  front_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  back_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  front_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  back_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

void brake_coast() {
  front_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  back_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  front_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  back_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void brake_brake() {
  front_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  back_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  front_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  back_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

// moves all motors at the same speed
void base_move(int speed) {
  front_left_mtr.move(speed);
  front_right_mtr.move(speed);
  back_left_mtr.move(speed);
  back_right_mtr.move(speed);
}

void base_move_encoders(int encoders) {
  front_left_mtr.move_absolute(encoders, 127);
  front_right_mtr.move_absolute(encoders, 127);
  back_left_mtr.move_absolute(encoders, 127);
  back_right_mtr.move_absolute(encoders, 127);
}