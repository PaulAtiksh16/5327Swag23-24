#include "main.h"
#include "pid.hpp"

void near_auton() {
    PID_controller piss(1300.0, 0.02, 6, 5);

    move_turn_pid(piss, 20);
    pros::delay(1000);
    move_lateral_pid(piss, -48);
}