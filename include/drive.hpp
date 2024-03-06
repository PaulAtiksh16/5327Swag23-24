#ifndef _DRIVE_HPP_
#define _DRIVE_HPP_

#ifdef cplusplus
extern "C" {
#endif

#include <stdint.h> // needed for the types declariations

#ifdef cplusplus
}
#endif

void tank_control(int left, int right);

double tank_control_curve(double joystick);

void arcade_control(int left, int right);

void get_base_watts(void);

void brake(void);
void right_brake(void);
void left_brake(void);

void brake_hold(void);

void brake_coast(void);

void brake_brake(void);

// function to make all motors move the same speed
void base_move(int speed);

void base_move_voltage(int voltage);
void left_move_voltage(int voltage);
void right_move_voltage(int voltage);

// functions for left and right motors to move independently
void left_move(int speed);

void right_move(int speed);

void base_move_encoders(int encoders);

void base_tare_position(void);

void base_move_relative(double encoders, int velocity);

#endif //_DRIVE_HPP_