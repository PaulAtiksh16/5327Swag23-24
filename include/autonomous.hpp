#ifndef _AUTONOMOUS_HPP_
#define _AUTONOMOUS_HPP_

#ifdef cplusplus
extern "C" {
#endif

#include <stdint.h> // needed for the types declariations
#ifdef cplusplus
}
#endif

void near_auton();
void far_auton();
void skills();
void new_skills();

#endif //_DRIVE_HPP_