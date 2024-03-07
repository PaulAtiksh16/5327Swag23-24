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
void wp_far();
void far_auton();
void push_one();
void true_skills();
void skills();
void new_skills();

#endif //_DRIVE_HPP_
