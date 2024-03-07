#include "drive.hpp"
#include "main.h"
#include "pros/motors.h"

extern pros::Motor front_left_mtr;
extern pros::Motor front_right_mtr;
extern pros::Motor back_left_mtr;
extern pros::Motor back_right_mtr;
extern pros::Motor top_left_mtr;
extern pros::Motor top_right_mtr;

extern pros::Motor left_side_motors;
extern pros::Motor right_side_motors;


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
    double new_left = tank_control_curve(left);
    double new_right = tank_control_curve(right);
    // actually runs the motors
  front_left_mtr.move(new_left);
  top_left_mtr.move(new_left);
  back_left_mtr.move(new_left);

  front_right_mtr.move(new_right);
  top_right_mtr.move(new_right);
  back_right_mtr.move(new_right);
//    left_side_motors.move(left);
//    right_side_motors.move(right);
}

double tank_control_curve(double joystick) {
  double tune_value = 2.4;
  return (std::pow(2.718, -(tune_value/10.0)) + std::pow(2.718, (fabs(joystick) - 127.0)/10.0 ) * (1 - std::pow(2.718, -(tune_value/10.0)))) * joystick;
    // Lukas code
    /*
    printf("left: %d, right: %d\n", left, right);

    int new_left = (std::pow(left, 3)) / (std::pow(127, 2));
    int new_right = (std::pow(right, 3)) / (std::pow(127, 2));

    printf("leftNEW: %d, rightNEW: %d\n", new_left, new_right);

    tank_control(new_left, new_right);


    tank_control(left, right);
    */
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
    pros::lcd::print(3, "left average: %f", (front_left_mtr.get_power() + back_left_mtr.get_power() + top_left_mtr.get_power())/3);
    pros::lcd::print(6, "right average: %f", (front_right_mtr.get_power() + back_right_mtr.get_power() + top_right_mtr.get_power())/3);

    pros::lcd::print(0, "front left: %f", front_left_mtr.get_power());
    pros::lcd::print(1, "back left: %f", back_left_mtr.get_power());
    pros::lcd::print(2, "top left: %f", top_left_mtr.get_power());

    pros::lcd::print(3, "front right: %f", front_right_mtr.get_power());
    pros::lcd::print(4, "back right: %f", back_right_mtr.get_power());
    pros::lcd::print(5, "top right: %f", top_right_mtr.get_power());
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