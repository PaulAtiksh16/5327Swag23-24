#include "drive.hpp"
#include "main.h"
#include "pros/motors.h"
#include <cmath>

extern pros::Motor front_left_mtr;
extern pros::Motor front_right_mtr;
extern pros::Motor back_left_mtr;
extern pros::Motor back_right_mtr;
extern pros::Motor top_left_mtr;
extern pros::Motor top_right_mtr;

// Omni Wheel diameter is 3.25 inches
const double wheel_radius = 1.625;
const double PI = 3.14159265358979323846;

double get_distance_in(double encoder_value)
{
    // Value in centidegrees, 100 centidegrees in 1 degree
    return ((encoder_value / 100) / 360) * (2 * PI * wheel_radius);
}

void pid(double distance_moved, double target_distance)
{
    
}
