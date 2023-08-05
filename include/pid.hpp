#ifndef _PID_HPP_
#define _PID_HPP_

#ifdef cplusplus
extern "C" {
#endif

#include <stdint.h> // needed for the types declariations

#ifdef cplusplus
}
#endif

// Function declarations here
double get_distance_in(double encoder_value);
void pid(double distance_moved, double target_distance);


#endif //_PID_HPP_
