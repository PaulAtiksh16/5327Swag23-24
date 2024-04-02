#include "main.h"
#include "drive.hpp"
#include "mechs.hpp"
#include "pid.hpp"
#include "pneumatics.hpp"
#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "autonomous.hpp"
#include "lemlib/api.hpp"


//HELPFUL LINKS:
//ascii text generator: https://fsymbols.com/generators/carty/
//PROS API: https://pros.cs.purdue.edu/v5/api/cpp/index.html

/**
CONTROLS:
Joysticks: roll around obviously
R1: Intake forward
R2: Intake backward
L2: Everything Off
R2: blocker toggle (out first push, back in second push)
X: walls toggle (out first push, back in second push)
UP: lift up
DOWN: lift down
LEFT: lift down; motor hold
RIGHT: lock toggle (out first push, back in second push)
*/

//CONTROLLER
pros::Controller master(pros::E_CONTROLLER_MASTER);

//MOTORS
pros::Motor front_left_mtr(FRONT_LEFT_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor top_left_mtr(MID_LEFT_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor back_left_mtr(BACK_LEFT_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor front_right_mtr(FRONT_RIGHT_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor top_right_mtr(MID_RIGHT_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor back_right_mtr(BACK_RIGHT_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::MotorGroup left_side_motors({front_left_mtr, top_left_mtr, back_left_mtr});
pros::MotorGroup right_side_motors({front_right_mtr, top_right_mtr, back_right_mtr});

pros::Imu inertial_sensor(INERTIAL_PORT);

pros::Motor intake_mtr(INTAKE_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor left_intake_mtr(LEFT_INTAKE_PORT, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_intake_mtr(RIGHT_INTAKE_PORT, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor flywheel_mtr(FLYWHEEL_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor flywheel_mtr_2(FLYWHEEL_PORT_2, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor left_lift_mtr(LEFT_LIFT_PORT, pros::E_MOTOR_GEAR_RED, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_lift_mtr(RIGHT_LIFT_PORT, pros::E_MOTOR_GEAR_RED, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor lift_mtr(LIFT_PORT, pros::E_MOTOR_GEAR_RED, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor conveyor_mtr(CONVEYOR_PORT, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor conveyor_mtr_2(CONVEYOR_PORT_2, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor slapper_mtr(SLAPPER_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);


//PNEUMATICS
pros::ADIDigitalOut left_wall(LEFT_WALL_PORT, false);
pros::ADIDigitalOut right_wall(RIGHT_WALL_PORT, false);

pros::ADIDigitalOut left_hang(LEFT_HANG_PORT, false);
pros::ADIDigitalOut right_hang(RIGHT_HANG_PORT, false);

pros::ADIDigitalOut lock(LOCK_PORT, false);
pros::ADIDigitalOut blocker(BLOCKER_PORT, false);

pros::ADIDigitalOut kicker(KICKER_PORT, false);

pros::ADIDigitalOut grabber(GRABBER_PORT, false);


lemlib::Drivetrain_t drivetrain{
        &left_side_motors, // left drivetrain motors
        &right_side_motors, // right drivetrain motors
        10.8, // track width
        3.25, // wheel diameter
        450 // wheel rpm,
        
};

lemlib::OdomSensors_t sensors{
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &inertial_sensor
};

// forward/backward PID
lemlib::ChassisController_t lateralController{
        12, // kP
        85, // kD
        1, // smallErrorRange
        100, // smallErrorTimeout
        3, // largeErrorRange
        500, // largeErrorTimeout
        50// slew rate
};

// turning PID
lemlib::ChassisController_t angularController{
        5, // kP
        20, // kD
        1, // smallErrorRange
        150, // smallErrorTimeout
        4, // largeErrorRange
        1000, // largeErrorTimeout
        50 // slew rate
};

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);

bool intakeState = false;

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
    // static bool pressed = false;
    // pressed = !pressed:
    // if (pressed) {
    // 	pros::lcd::set_text(2, "I was pressed!");
    // } else {
    // 	pros::lcd::clear_line(2);
    // }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    chassis.calibrate();

    pros::lcd::initialize();
    pros::lcd::set_text(1, "trois balls");

    pros::lcd::register_btn1_cb(on_center_button);

    // pros::Task screenTask(screen); // create a task to print the position to the screen

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    everything_off();
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {

}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

extern bool lock_flag;

extern bool intake_flag;

void autonomous() {
    // near_auton();
    true_skills();
}


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    brake_coast();

    // unlock lift at start of  control
    lock.set_value(true);
    lock_flag = true;

    bool lift_state = false;

    pros::Task task{[=] {
        while (true) {
            int left = master.get_analog(ANALOG_LEFT_Y);
            int right = master.get_analog(ANALOG_RIGHT_X);
            arcade_control(left, right);
            pros::delay(20);
        }
    }};

    pros::Task intake_hold_task{[=] {
        while (true) {
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
                intake_mtr.move(-127);
            }
            pros::delay(20);
        }
        intake_mtr.move(127);
    }};

    task.resume();

    bool holdState = false;
    bool forward = false;

    while (true) {

        // TANK DRIVE //
//        int left = master.get_analog(ANALOG_LEFT_Y);
//        int right = master.get_analog(ANALOG_RIGHT_Y);
        // tank_control(left, right);

//        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
//            chassis.setPose({0, 0, 0});
//            chassis.moveTo(0, 48, 3000);
//        }

        // ARCADE DRIVE //
        // int left = master.get_analog(ANALOG_LEFT_Y);
        // int right = master.get_analog(ANALOG_RIGHT_X);
        // arcade_control(left, right);

        // SINGLE STICK ARCADE //
        // int left = master.get_analog(ANALOG_LEFT_Y);
        // int right = master.get_analog (ANALOG_LEFT_X);
        // arcade_control(left, right);



        // // LIFT UP //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        // 	if (lift_state == false) {
        // 		lift_state = true;
        // 		move_lift(lift_state);
        // 	}
        // }

        // // LIFT DOWN //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        // 	if (lift_state == true) {
        // 		lift_state = false;
        // 		move_lift(lift_state);
        // 	}
        // }


        // // INTAKE //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        //     run_intake_forward();
        //     // intake();
        //     // pros::delay(200);
        // }

        // // OUTTAKE //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        //     run_intake_backward();
        //     // outtake();
        //     // pros::delay(200);
        // }
        // INTAKE TOGGLE //
        /*
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            holdState = false;
            intake_toggle();
        }
        */

        
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            // holdState = true;
            // forward = true;
            // intake_hold(1);
            // } else if (forward == true && holdState == true) {
            //     intake_hold(-1);
            //     forward = false;
            intake_mtr.move(-127);
            intake_flag = false;
            

            forward = true;



        } else {
            if (forward == true) {
                intake_mtr.move(127);
                intake_flag = true;
                
            }

            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
                forward = false;
                intake_toggle();
            }
            pros::delay(200);
        }


        
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        //     intake_toggle();
        // }
        // INTAKE HOLD //
        /*
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            holdState = true;
        }
        */
        

        

        // FLYWHEEL //
//        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
//        	run_flywheel(127);
//        }

        // FLYWHEEL OFF //
//        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
//        	run_flywheel(0);
//        }

        // // EVERYTHING OFF //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        //     everything_off();
        // }

        //  //HANG UP
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        //     hang_toggle();
        //     pros::delay(200);
        // }
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
        //     slapper();
        //     pros::delay(200);
        // }


        // LIFT UP //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        //     move_lift_up();
        // }

        // LIFT DOWN //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        //     move_lift_down();
        // }

        // LIFT HOLD //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        //     move_lift_hang();
        // }

        // INDEX //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        // 	index();
        // }

        // WALLS TOGGLE //
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            walls_toggle();
            pros::delay(200);
        }

        // BLOCKER TOGGLE //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        //     blocker_toggle();
        //     pros::delay(200);
        // }

        // LOCK TOGGLE //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        //     lock_toggle();
        //     pros::delay(500);
        //     // move_lift_end();
        //     // pros::delay(250);
        // }

        // KICKER MACRO //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        // 	// kick_macro();
        // 	pros::Task kicker_task(kick_macro);
        // }

        // KICKER TOGGLE //
        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        // 	kick_toggle();
        // }

        // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        // 	// kick_macro();
        // 	grabber_toggle();
        // 	pros::delay(200);
        // }



        // DEBUGGING //
        get_base_watts();
        // get_mech_watts();

        lemlib::Pose currentPose = chassis.getPose();

        float x = currentPose.x;
        float y = currentPose.y;
        float theta = currentPose.theta;
//
//        pros::lcd::set_text(2, "x: " + std::to_string(x));
//        pros::lcd::set_text(3, "y: " + std::to_string(y));
//        pros::lcd::set_text(4, "theta: " + std::to_string(theta));
        pros::lcd::set_text(1, "tl: " + std::to_string(top_left_mtr.get_position()));
        pros::lcd::set_text(2, "tr: " + std::to_string(top_right_mtr.get_position()));
        pros::lcd::set_text(3, "bl: " + std::to_string(back_left_mtr.get_position()));
        pros::lcd::set_text(4, "br: " + std::to_string(back_right_mtr.get_position()));
        pros::lcd::set_text(5, "fl: " + std::to_string(front_left_mtr.get_position()));
        pros::lcd::set_text(6, "fr: " + std::to_string(front_right_mtr.get_position()));


        pros::delay(20);
    }
}
