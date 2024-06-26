#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "drive.hpp"
#include "mechs.hpp"
#include "pid.hpp"
#include "pneumatics.hpp"
#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/rtos.hpp"
#include "autonomous.hpp"
#include "lemlib/api.hpp"

extern lemlib::Chassis chassis;

void alliance() {
    chassis.setPose(0, 0, 135);
    chassis.turnTo(-8, 12, 600, true);
    chassis.moveTo(-12, 12, 1000);
    chassis.moveTo(-12, 28, 3500, 200);
    chassis.moveTo(-12, 20, 1000);
    chassis.moveTo(-12, 28, 1000);
}


void near_auton() {
    // Bot at same angle as match load bar, folding intake facing wall, back right tile on corner
    chassis.setPose(0, 0, 225);
    // Move backwards slightly
    chassis.moveTo(5,5, 1000);
    walls_toggle();
    chassis.moveTo(-2, -1.5, 1000);
    run_intake_backward();
    // Turn to face elevation bar
    chassis.turnTo(-3, -25, 1000);

    chassis.turnTo(50, 0, 1000, false, 75);
    pros::delay(200);
    chassis.turnTo(-6, -25, 1000);

    walls_toggle();
    // Move towards elevation bar
    chassis.moveTo(-4, -35, 2000);
    pros::delay(500);
    run_intake_backward();
}

void wp_far() {
   // Bot at same angle as match load bar, folding intake facing wall, back right tile on corner
    chassis.setPose(0, 0, 225);
    // Move backwards slightly
    chassis.moveTo(4,4, 1000);
    walls_toggle();
    chassis.moveTo(-2, -1, 1000);
    run_intake_backward();
    // Turn to face elevation bar
    chassis.turnTo(-3, -25, 1000);
    left_wall_toggle();
    // Move towards elevation bar
    chassis.moveTo(-3, -34, 2000);
    pros::delay(500);
    run_intake_backward(); 
}

void push_one() {
    // move forward to push one ball
    chassis.setPose(0, 0, 0);
    chassis.moveTo(0, -50, 3000);

}


void far_auton() {
    // Bot at same angle as match load bar, facing goal, front left tip on corner
    chassis.setPose(0.0, 0.0, 315.0);

    // Release intake;
    walls_toggle();
    run_intake_backward();
    // Push matchload triball
    chassis.moveTo(-12, 10.5, 600, 100);
    chassis.moveTo(-23, 7, 1000, 100);
    walls_toggle();
    run_intake_backward();

    // Push triballs a second time
    chassis.turnTo(-15, 12, 650, true);
    chassis.moveTo(-15, 12, 650, 60);
    chassis.turnTo(-30, 18, 650, true, 90);
    chassis.moveTo(-30, 18, 650, 200);

    // Get first triball at barrier and outtake
    chassis.moveTo(-13, 9, 900, 100);
    chassis.turnTo(-35, -32, 500, false, 100);
    run_intake_forward();
    chassis.moveTo(-26, -32, 3000, 100);

    //grab balls
    pros::delay(500);

    // Turn to goal
    chassis.turnTo(-45, 5, 700, false, 70);
    chassis.moveTo(-45, -10, 800);
    run_intake_backward();
    pros::delay(200);
    chassis.moveTo(-35, -22, 800);
    run_intake_backward();

    // Grab second triball
    chassis.turnTo(-48, -42, 1000, false, 100);
    run_intake_forward();
    chassis.moveTo(-48, -42, 1000, 85);

    pros::delay(300); // Slight delay to grip triball

    // Push three triballs into goal
    chassis.turnTo(-55, 4, 1000, false, 90);
    run_intake_forward();
    run_intake_backward();
    walls_toggle();

    // Hit wall lol
    base_move(200);
    pros::delay(700);
    base_move(-200);
    pros::delay(150);
    base_move(200);
    pros::delay(200);
    base_move(0);
    walls_toggle();
    run_intake_backward();
}

void true_skills() {
    chassis.setPose(0.0, 0.0, 135.0);

    // Push alliance triballs into blue goal
    chassis.turnTo(-50, 50, 1000, true);
    chassis.moveTo(-6, 6, 1000);
    chassis.turnTo(-10, 25, 1000, true);
    base_move(-87);
    pros::delay(1000);
    base_move(-20);
    pros::delay(300);
    base_move(0);

    // Re-orient bot to be origin and have back face goal
    chassis.setPose(0, 0, 180.0);

    // Move back to touch matchload bar
    chassis.turnTo(30, -30, 500);
    chassis.moveTo(6, -6, 1000);
    chassis.turnTo(-30, -20, 500, true);
    chassis.moveTo(-6, -7, 1000, 40);


    pros::delay(1000); // temporary ball drop
    // Shoot dem ballz
    /*
    slapper();
    pros::delay(30000);
    slapper();
    */

    int alleyY = -20;
    // Move towards alley
    chassis.moveTo(24, alleyY, 1500);
    // Move through alley
    chassis.moveTo(96, alleyY, 5000);

    // * REORIENT AFTER EVERY PUSH LOL *
    run_intake_backward();
    
    // Make barrier push and turn bot around
    int goalX = 96+20;
    int goalY = alleyY+26.5;
    // turn towards goal
    chassis.turnTo(goalX, alleyY+7, 2000, true, 60); // intermediate movement
    chassis.moveTo(goalX, alleyY+7, 1000, 60);
    chassis.turnTo(goalX+8, goalY, 1000, true);
    // Ram into goal
    base_move(-127);
    pros::delay(1000);
    base_move(0);
    // Re-orient bot to be origin and have back face goal
    chassis.setPose(0, 0, 180.0);

    // First front push
    chassis.moveTo(0, -18, 700); // back out slightly
    int intermediate1[2] = {-45, 4};
    chassis.turnTo(intermediate1[0], intermediate1[1], 1000);
    chassis.moveTo(intermediate1[0], intermediate1[1], 4000, 60); 

    int front1[2] = {6, 12};
    chassis.turnTo(front1[0], front1[1], 1000, false, 60); 
    walls_toggle(); // activating wings 

    base_move(120);
    pros::delay(1500); // push on barrier
    base_move(0);

    chassis.setPose(0, 0, 90);

    // Second front push
    walls_toggle(); // turn off walls
    int intermediate2[2] = {-40, 2};
    chassis.moveTo(intermediate2[0], 0, 1000); // back out of goal
    walls_toggle(); // turn on walls

    int front2[2] = {0, 40};
    walls_toggle(); // turn off walls
    chassis.turnTo(intermediate2[0], front2[1]/2.0, 1000);
    chassis.moveTo(intermediate2[0], front2[1]/2.0, 1000);
    chassis.turnTo(front2[0], front2[1], 1000);

    walls_toggle(); // turn on walls
    base_move(80);
    pros::delay(2000);
    base_move(-50);
    pros::delay(300);
    base_move(127);
    pros::delay(1000);  
    base_move(0);
    
    chassis.setPose(0, 0, 90);

    // Rotate around to make final push
}

void skills() {
    // Bot at same angle as match load bar, facing goal, front left tip on corner
    
    chassis.setPose(0, 0, 135);

    // Push alliance triballs into blue goal
    chassis.turnTo(-60, 60, 1000, true);
    chassis.moveTo(-6, 6, 1000, true);
    chassis.moveTo(-10, 25, 1000, true);

    // Get to matchloading position

    /*
    chassis.setPose(-58, -40, 62);

    // Go to bottom left barrier corner
    chassis.moveTo(-14, -27, 1500);
    walls_toggle();
    run_intake_backward();

    // Move toward top of barrier
    chassis.moveTo(-15, 39, 4000, 180);

    //Scoop balls to left of barrier
    chassis.turnTo(-50, 48, 1000, false, 40);

    // Move to top left corner
    chassis.moveTo(-45, 50, 2000, 40);

    //Go toward entrance of alley
    chassis.turnTo(-18, 74, 3000, false, 80);
    chassis.moveTo(-18, 77, 3000, 90);
    chassis.moveTo(-18, 79, 3000, 100);

    //move to top right corner
    chassis.moveTo(30, 79, 4000, 120);
    chassis.moveTo(38, 80, 4000, 100);

    //move balls to score to goal
    chassis.moveTo(62, 57, 1000, 100);
    run_intake_backward();
    chassis.turnTo(65, 57, 500, true, 100);
    chassis.moveTo(65, 57, 600, 127);

    //curve to push balls into goal
    chassis.turnTo(45, 57, 500, 100);
    chassis.moveTo(45, 57, 500, 100);
    chassis.moveTo(45, 47, 500, 127);
    */

    /*   
    chassis.setPose(0, 0, -29);
    run_flywheel(127);
    pros::delay(30000);
    run_flywheel(0);

    // go to other side of field
    chassis.turnTo(0, 92.5, 1000);
    chassis.moveTo(0, 92.5, 4000);

    // go closer to goal
    chassis.turnTo(-10, 100, true);
    chassis.moveTo(-10, 100, 1000);
    // push into goal side
    chassis.turnTo(-38, 110, 1000, true);
    chassis.moveTo(-38, 110, 1000);
    // Position ourselves away from goal
    chassis.moveTo(-15, 100, 1000);
    walls_toggle();
    // Turn away from goal
    chassis.turnTo(-30, 70, 1000);
    chassis.moveTo(-30, 70, 1000);


    chassis.turnTo(-50, 55, 1000, false, 40);
    chassis.moveTo(-50, 55, 1000, 40);

    chassis.turnTo(-60, 55, 1000, false, 40);
    chassis.moveTo(-60, 55, 1000, 40);

    // push middle into goal
    chassis.turnTo(-60, 100, 1000, false, 40);
    chassis.moveTo(-60, 100, 1000);
    pros::delay(500);
    walls_toggle();

    //back up from wall then turn toward middle
    chassis.moveTo(-60, 55, 1000, 60);
    walls_toggle();
    chassis.moveTo(-60, 100, 1000);
    */
//    chassis.turnTo(-70, 60, 1000);

//    // open wings
//    walls_toggle();
//
//    //back to middle
//    chassis.moveTo(-70, 60, 1000, 40);
//
//    //turn toward right corner
//    chassis.turnTo(-80, 70, 1000);
//    chassis.moveTo(-80, 70, 1000);
//
//    //turn toward goal again
//    chassis.turnTo(-70, 100, 1000);
//    chassis.moveTo(-70, 100, 1000);
//    pros::delay(500);
//    walls_toggle();
//
//    // Move away from goal
//    chassis.turnTo(-70, 70, 1000);
//    chassis.moveTo(-70, 70, 1000);
//    // Move towards goal's side
//    chassis.turnTo(-100, 90, 1000);
//    chassis.moveTo(-100, 90, 1000);
//    // Move to goal's side
//    chassis.moveTo(-100, 110, 1000);
//    // Push with back of bot
//    walls_toggle();
//    chassis.turnTo(-100, 110, 1000);
//    chassis.moveTo(-70, 110, 1000);


//    chassis.turnTo(-90, 70, 1000);
//    chassis.moveTo(-90, 70, 1000);

    //put back into



    // Move away, move to my left, push again




//    // push middle into goal
//    chassis.moveTo(-50, 120, 1000);
//    //backup
//    walls_toggle();
//    chassis.moveTo(-50, 50, 1000, true);
//    chassis.turnTo(-70, 50, 1000);
//    walls_toggle();


}

//void new_skills()
//{
//    chassis.setPose(-48, -57, 0);
//    chassis.follow("skills_goofy3.txt", 30000, 50);
//    pros::delay(20000);
//}
