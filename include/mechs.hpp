#ifndef _MECHS_HPP_
#define _MECHS_HPP_

#ifdef cplusplus
extern "C" {
#endif

#include <stdint.h> // needed for the types declariations

#ifdef cplusplus
}
#endif

void everything_off(void);

void get_mech_watts(void);

void run_intake_forward(void);
void run_intake_backward(void);

void intake_toggle();
void intake_hold(int sign);

void run_flywheel(int power);

void move_lift(bool lift_state);

void move_lift_up(void);
void move_lift_down(void);
void move_lift_hang(void);
void move_lift_end(void);

void run_conveyor_forward(void);
void run_conveyor_backward(void);

void intake(void);
void outtake(void);
void intakeHold(void);

void index(void);

void slapper(void);

#endif //_MECHS_HPP_
