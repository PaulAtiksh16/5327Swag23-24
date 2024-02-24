#ifndef _PNEUMATICS_HPP_
#define _PNEUMATICS_HPP_

#ifdef cplusplus
extern "C" {
#endif

#include <stdint.h> // needed for the types declariations

#ifdef cplusplus
}
#endif

void walls_toggle(void);
void right_wall_toggle(void);

void hang_toggle(void);

void kick_macro(void);
void kick_toggle(void);

void grabber_toggle(void);

void blocker_toggle(void);

void lock_toggle(void);
void left_wall_toggle(void);

#endif //_MECHS_HPP_
