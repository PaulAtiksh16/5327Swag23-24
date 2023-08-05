#include "mechs.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

extern pros::ADIDigitalOut left_wall;
extern pros::ADIDigitalOut right_wall;

extern pros::ADIDigitalOut kicker;

extern pros::ADIDigitalOut grabber;

bool grabber_flag = false;

bool walls_flag = false;

bool kicker_flag = false;

//open walls
void walls_toggle() {
    walls_flag = !walls_flag;

    if (walls_flag) {
        //walls expand when piston actuates
        left_wall.set_value(true);
        right_wall.set_value(true);
    } else {
        //walls fold when piston retracts
        left_wall.set_value(false);
        right_wall.set_value(false);
    }
}

//actuate and retract kicker
void kick_macro() {
  kicker_flag = true;
  kicker.set_value(false); //bands snap forward when piston turns off
  pros::delay(250);
  kicker.set_value(true); //bands retract when piston turns on
  kicker_flag = false;
}

void kick_toggle() {
    kicker_flag = !kicker_flag;

    if (kicker_flag) {
        kicker.set_value(false); //bands snap forward when piston turns off
    } else {
        kicker.set_value(true); //bands retract when piston turns on
    }
}

void grabber_toggle() {
    grabber_flag = !grabber_flag;

    if (grabber_flag) {
        grabber.set_value(true); //grabber closes when piston turns on
    } else {
        grabber.set_value(false); //grabber opens when piston turns off
    }
}