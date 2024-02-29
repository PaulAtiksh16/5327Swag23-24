#include "drive.hpp"
#include "main.h"
#include "pros/motors.h"

extern pros::Motor front_left_mtr;
extern pros::Motor front_right_mtr;
extern pros::Motor back_left_mtr;
extern pros::Motor back_right_mtr;
extern pros::Motor top_left_mtr;
extern pros::Motor top_right_mtr;

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
    top_left_mtr.move(left);

    front_right_mtr.move(right);
    back_right_mtr.move(right);
    top_right_mtr.move(right);
}

void tank_control_curve(int left, int right) {


    int x[18] = {10, 15, 20, 35, 40, 50, 60, 75, 90, 100, 105, 110, 112, 115, 118, 120, 122, 150};
    int voltage[17] = {11, 15, 25, 35, 45, 55, 60, 65, 70, 80, 90, 95, 100, 105, 110, 115, 127};

    for (int i = 1; i < 18; i++) {
      if (abs(left) <= x[i] && abs(left) > x[i-1]) {
        if (left < 0)
        {
          left = voltage[i] * -1;
          break;
        }
        else
        {
          left = voltage[i];
          break;
        }
      }
      
    }


    for (int i = 1; i < 18; i++) {
      if (abs(right) <= x[i] && abs(right) > x[i-1]) {
        if (right < 0)
        {
          right = voltage[i] * -1;
          break;
        }
        else
        {
          right = voltage[i];
          break;
        }
      }
      
    }


    tank_control(left, right);
}

void arcade_control(int left, int right) {

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
    left_move((left+right < 127) ? (left+right) : 127);
    right_move(left-right);
}

//display wattage from base
void get_base_watts() {
    pros::lcd::print(0, "front left: %f", front_left_mtr.get_power());
    pros::lcd::print(1, "back left: %f", back_left_mtr.get_power());
    pros::lcd::print(2, "top left: %f", top_left_mtr.get_power());
    pros::lcd::print(3, "left average: %f", (front_left_mtr.get_power() + back_left_mtr.get_power() + top_left_mtr.get_power())/3);

    pros::lcd::print(3, "front right: %f", front_right_mtr.get_power());
    pros::lcd::print(4, "back right: %f", back_right_mtr.get_power());
    pros::lcd::print(5, "top right: %f", top_right_mtr.get_power());
    pros::lcd::print(6, "right average: %f", (front_right_mtr.get_power() + back_right_mtr.get_power() + top_right_mtr.get_power())/3);
}

//braking
void brake() {
  front_left_mtr.brake();
  back_left_mtr.brake();
  front_right_mtr.brake();
  back_right_mtr.brake();
  top_left_mtr.brake();
  top_right_mtr.brake();
}

void right_brake() {
  front_right_mtr.brake();
  back_right_mtr.brake();
  top_right_mtr.brake();
}

void left_brake() {
  front_left_mtr.brake();
  back_left_mtr.brake();
  top_left_mtr.brake();
}


// setting brake modes
void brake_hold() {
  front_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  back_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  front_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  back_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  top_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  top_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

void brake_coast() {
  front_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  back_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  front_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  back_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  top_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  top_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void brake_brake() {
  front_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  back_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  front_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  back_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  top_left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  top_right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

// moves all motors at the same speed
void base_move(int speed) {
  front_left_mtr.move(speed);
  front_right_mtr.move(speed);
  back_left_mtr.move(speed);
  back_right_mtr.move(speed);
  top_right_mtr.move(speed);
  top_left_mtr.move(speed);
}

void base_move_voltage(int voltage) {
  front_left_mtr.move_voltage(voltage);
  front_right_mtr.move_voltage(voltage);
  back_left_mtr.move_voltage(voltage);
  back_right_mtr.move_voltage(voltage);
  top_right_mtr.move_voltage(voltage);
  top_left_mtr.move_voltage(voltage);
}

void left_move_voltage(int voltage) {
  front_left_mtr.move_voltage(voltage);
  back_left_mtr.move_voltage(voltage);
  top_left_mtr.move_voltage(voltage);
}

void right_move_voltage(int voltage) {
  front_right_mtr.move_voltage(voltage);
  back_right_mtr.move_voltage(voltage);
  top_right_mtr.move_voltage(voltage);
}

// move left side
void left_move(int speed) {
  front_left_mtr.move(speed);
  back_left_mtr.move(speed);
  top_left_mtr.move(speed);
}

// move right side
void right_move(int speed) {
  front_right_mtr.move(speed);
  back_right_mtr.move(speed);
  top_right_mtr.move(speed);
}

void base_move_encoders(int encoders) {
  front_left_mtr.move_absolute(encoders, 127);
  front_right_mtr.move_absolute(encoders, 127);
  back_left_mtr.move_absolute(encoders, 127);
  back_right_mtr.move_absolute(encoders, 127);
  top_left_mtr.move_absolute(encoders, 127);
  top_right_mtr.move_absolute(encoders, 127);
}

//reset motor encoder positions
void base_tare_position() {
  front_left_mtr.tare_position();
	front_right_mtr.tare_position();
	back_left_mtr.tare_position();
	back_right_mtr.tare_position();
	top_left_mtr.tare_position();
	top_right_mtr.tare_position();
}

void base_move_relative(double encoders, int velocity) {
  front_left_mtr.move_relative(encoders, velocity);
  front_right_mtr.move_relative(encoders, velocity);
  back_left_mtr.move_relative(encoders, velocity);
  back_right_mtr.move_relative(encoders, velocity);
  top_left_mtr.move_relative(encoders, velocity);
  top_right_mtr.move_relative(encoders, velocity);
}