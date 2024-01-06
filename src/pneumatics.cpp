#include "mechs.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

extern pros::ADIDigitalOut left_wall;
extern pros::ADIDigitalOut right_wall;

extern pros::ADIDigitalOut left_hang;
extern pros::ADIDigitalOut right_hang;

extern pros::ADIDigitalOut kicker;

extern pros::ADIDigitalOut grabber;

extern pros::ADIDigitalOut blocker;

extern pros::ADIDigitalOut lock;

bool grabber_flag = false;

bool walls_flag = false;

bool hang_flag = false;

bool kicker_flag = false;

bool blocker_flag = false;

bool lock_flag = false;

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

void right_wall_toggle() {
    walls_flag = !walls_flag;

    if (walls_flag) {
        //walls expand when piston actuates
        right_wall.set_value(true);
    } else {
        //walls fold when piston retracts
        right_wall.set_value(false);
    }
}

void hang_toggle() {
    hang_flag = !hang_flag;

    if (hang_flag) {
        //hang lifts when piston actuates
        left_hang.set_value(true);
        right_hang.set_value(true);
    } else {
        //hang folds when piston retracts
        left_hang.set_value(false);
        right_hang.set_value(false);
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
        grabber.set_value(true); //grabber deploys when piston turns on
    } else {
        grabber.set_value(false); //grabber retracts when piston turns off
    }
}


void blocker_toggle() {
    blocker_flag = !blocker_flag;

    if (blocker_flag) {
        blocker.set_value(true); //blocker deploys when piston turns on
    } else {
        blocker.set_value(false); //blocker retracts when piston turns off
    }
}

void lock_toggle() {
    lock_flag = !lock_flag;

    if (lock_flag) {
        lock.set_value(true); //lock deploys when piston turns on
    } else {
        lock.set_value(false); //lock retracts when piston turns off
    }
}