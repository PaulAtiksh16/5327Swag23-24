#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "drive.hpp"
#include "mechs.hpp"
#include "okapi/api/chassis/controller/chassisControllerPid.hpp"
#include "okapi/api/control/iterative/iterativeVelPidController.hpp"
#include "pid.hpp"
#include "pneumatics.hpp"
#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/rtos.hpp"
#include "autonomous.hpp"
#include "lemlib/api.hpp"

extern lemlib::Chassis chassis;

void near_auton() {
    // Bot at same angle as match load bar, folding intake facing wall, back right tile on corner
    chassis.setPose(0, 0, 225);
    // Move backwards slightly
    chassis.moveTo(4,4, 1000);
    walls_toggle();
    chassis.moveTo(-3, 0, 1000);
    run_intake_backward();
    // Turn to face elevation bar
    chassis.turnTo(-3, -10, 1000);
    // Move towards elevation bar
    chassis.moveTo(-3, -39, 2000);
    pros::delay(500);
    walls_toggle();
    run_intake_backward();
}

void far_auton() {
    // Bot at same angle as match load bar, facing goal, front left tip on corner
    chassis.setPose(0, 0, 315);
    // Release intake;
    walls_toggle();
    run_intake_backward();
    // Push matchload triball
    chassis.moveTo(-8, 8, 600);
    chassis.moveTo(-30, 8, 1000);
    walls_toggle();
    // Back push to get triballs in goal
    /*
    chassis.moveTo(-20, 18, 1000);
    chassis.turnTo(-28, 23, 1000, true);
    chassis.moveTo(-28, 23, 1000);
    */
    run_intake_backward();
    chassis.moveTo(-20, 23, 1000);
    chassis.turnTo(-32, 23, 800, true);
    chassis.moveTo(-32, 23, 1000);

    // Get first triball at barrier and outtake

    chassis.moveTo(-14, 23, 1000);
    chassis.turnTo(-35, -20, 500);
    run_intake_forward();
    chassis.moveTo(-30, -20, 3000);
    pros::delay(500 );
    chassis.turnTo(-45, 5, 800);
    run_intake_forward();
    run_intake_backward();
    pros::delay(300);
    run_intake_backward();

    // Grab second triball

    chassis.turnTo(-60, -34, 500);
    run_intake_forward();
    chassis.moveTo(-60, -34, 1000);

    // Push three triballs into goal
    chassis.turnTo(-45, 5, 750);
    run_intake_backward();
    walls_toggle();
    //chassis.moveTo(-60, 5, 2000);
    base_move(200);
    pros::delay(1000);
    base_move(0);
    walls_toggle();
    run_intake_backward();
}

void skills() {
    chassis.setPose(0, 0, -27);
    run_flywheel(127);
    pros::delay(2000);
    run_flywheel(0);
    walls_toggle();
    // go to other side of field
    chassis.turnTo(0, 92.5, 1000);
    chassis.moveTo(0, 92.5, 4000);
    walls_toggle();
    // go closer to goal
    chassis.turnTo(-10, 100, true);
    chassis.moveTo(-10, 100, 1000);
    // push into goal side
    chassis.turnTo(-38, 110, 1000, true);
    chassis.moveTo(-38, 110, 1000);
    // Position ourselves away from goal
    chassis.moveTo(-25, 100, 1000);
    walls_toggle();
    // Turn away from goal
    chassis.turnTo(-34, 70, 1000);
    chassis.moveTo(-34, 70, 1000);


    chassis.turnTo(-50, 55, 1000);
    chassis.moveTo(-50, 55, 1000);

    chassis.turnTo(-60, 55, 1000);
    chassis.moveTo(-60, 55, 1000);

    chassis.turnTo(-60, 100, 1000);
    chassis.moveTo(-60, 100, 1000);
    walls_toggle();

    // Move away, move to my left, push again
    



//    // push middle into goal
//    chassis.moveTo(-50, 120, 1000);
//    //backup
//    walls_toggle();
//    chassis.moveTo(-50, 50, 1000, true);
//    chassis.turnTo(-70, 50, 1000);
//    walls_toggle();


}