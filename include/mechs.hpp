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

void run_flywheel(void);

void move_lift(int lift_state);

#endif //_MECHS_HPP_
