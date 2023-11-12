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


//HELPFUL LINKS:
//ascii text generator: https://fsymbols.com/generators/carty/
//PROS API: https://pros.cs.purdue.edu/v5/api/cpp/index.html

/**
CONTROLS:
Joysticks: roll around obviously
A: Intake forward
B: Intake backward
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
pros::Motor top_left_mtr(MID_LEFT_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor back_left_mtr(BACK_LEFT_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor front_right_mtr(FRONT_RIGHT_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor top_right_mtr(MID_RIGHT_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor back_right_mtr(BACK_RIGHT_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::MotorGroup left_side_motors({front_left_mtr, top_left_mtr, back_left_mtr});
pros::MotorGroup right_side_motors({front_right_mtr, top_right_mtr, back_right_mtr});

pros::Imu inertial_sensor(INERTIAL_PORT);

pros::Motor intake_mtr(INTAKE_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor left_intake_mtr(LEFT_INTAKE_PORT, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_intake_mtr(RIGHT_INTAKE_PORT, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor flywheel_mtr(FLYWHEEL_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor flywheel_mtr_2(FLYWHEEL_PORT_2, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor left_lift_mtr(LEFT_LIFT_PORT, pros::E_MOTOR_GEAR_RED, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_lift_mtr(RIGHT_LIFT_PORT, pros::E_MOTOR_GEAR_RED, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor lift_mtr(LIFT_PORT, pros::E_MOTOR_GEAR_RED, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor conveyor_mtr(CONVEYOR_PORT, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor conveyor_mtr_2(CONVEYOR_PORT_2, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);


//PNEUMATICS
pros::ADIDigitalOut left_wall(LEFT_WALL_PORT, false);
pros::ADIDigitalOut right_wall(RIGHT_WALL_PORT, false);

pros::ADIDigitalOut lock(LOCK_PORT, false);
pros::ADIDigitalOut blocker(BLOCKER_PORT, false);

pros::ADIDigitalOut kicker(KICKER_PORT, false);

pros::ADIDigitalOut grabber(GRABBER_PORT, false);


lemlib::Drivetrain_t drivetrain {
    &left_side_motors, // left drivetrain motors
    &right_side_motors, // right drivetrain motors
    10.5, // track width
    3.25, // wheel diameter
    360 // wheel rpm
};

lemlib::OdomSensors_t sensors {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &inertial_sensor
};

// forward/backward PID
lemlib::ChassisController_t lateralController {
    10, // kP
    70, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    1 // slew rate
};
 
// turning PID
lemlib::ChassisController_t angularController {
    7.2, // kP
    60, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    0 // slew rate
};

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	// static bool pressed = false;
	// pressed = !pressed;
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
	pros::lcd::set_text(1, "hi bitchy!");

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
using namespace okapi;

extern bool lock_flag;
void autonomous() {
	
	// CLOSE AUTON //
	lock.set_value(true);
	lock_flag = true;

	chassis.setPose(0, 0, 45);
	chassis.moveTo(2, 2, 1000);

	// Use walls to push match load triball
	walls_toggle();
	pros::delay(500);
	walls_toggle();
    // Push triball
	chassis.moveTo(0, 0, 1000);
	// chassis.turnTo(24, 12, 500);
	// chassis.setPose(-1.4, -1.4, 24.444);
	
	chassis.moveTo(22.5, 12, 2000);
    pros::delay(400);
	// Outtake
	run_intake_backward();
	pros::delay(500);
	run_intake_backward();

	// Move away to Push Triball
	chassis.moveTo(16, 12, 1000);

    // rotate 180 degrees
    chassis.turnTo(0, 12, 1000);
	// Push triball
	chassis.moveTo(30, 12, 1000);
    pros::delay(400);

	// Move towards matchload
	chassis.turnTo(15, -7, 1000);
	chassis.moveTo(15, -7, 2000);

	// Face triball
	chassis.turnTo(-3, -10, 1000);
	chassis.moveTo(-3, -10, 2500);

    // Move towards elevation bar (turn bot so blocker works lol)
	chassis.turnTo(-5, -39, 500);
    chassis.moveTo(-5, -39, 2000);
    // Edge the robot
	chassis.moveTo(-5, -41.5, 1000);
	
	
	//Far Auton
	// lock.set_value(true);
	// lock_flag = true;

	// chassis.setPose(0, 1, -45);

	// chassis.moveTo(6.5, -6.5, 1000);
	// walls_toggle();
	// pros::delay(500);
	// walls_toggle();
	// chassis.moveTo(15, -15, 1000);
	// chassis.moveTo(100, -25, 1500);
	// chassis.moveTo(15, -15, 1000);



	/*
	// ** SKILLS RUN ** //
	chassis.setPose(0, -1, 0);
	// Blocker on, match load, blocker off
	lock.set_value(true);
	lock_flag = true;

	bool lift_state = false;

	move_lift_up();
	pros::delay(30000);
	move_lift_down();
	// Walls on
	walls_toggle();
	chassis.moveTo(0, -84, 4000);

	chassis.setPose(0, 0, 0);
	chassis.turnTo(30, -27, true);
	chassis.moveTo(30, -27, 2000);

	// Move back then push rly hard lol
	chassis.moveTo(10, -10, 1000);

	chassis.turnTo(50, -37, true);
	chassis.moveTo(50, -37, 1000);
	walls_toggle();
	chassis.moveTo(20, -10, 1000);
	*/
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
	
	// unlock lift at start of  control
	lock.set_value(true);
	lock_flag = true;

	bool lift_state = false;

	while (true) {

		// TANK DRIVE //
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);
		tank_control(left, right);

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


		// INTAKE //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
			run_intake_forward();
			// intake();
		}
		
		// OUTTAKE //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
			run_intake_backward();
			// outtake();
		}

		// FLYWHEEL //
		// if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
		// 	run_flywheel(115);
		// }

		// FLYWHEEL OFF //
		// if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
		// 	run_flywheel(0);
		// }

		// EVERYTHING OFF //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			everything_off();
		}


		// LIFT UP //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
			move_lift_up();
		}

		// LIFT DOWN //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
			move_lift_down();
		}

		// LIFT HOLD //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
			move_lift_hang();
		}

		// INDEX // 
		// if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
		// 	index();
		// }

		// WALLS TOGGLE //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
			walls_toggle();
			pros::delay(200);
		}

		// BLOCKER TOGGLE //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			blocker_toggle();
			pros::delay(200);
		}

		// LOCK TOGGLE //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
			lock_toggle();
			pros::delay(500);
			// move_lift_end();
			// pros::delay(250);
		}

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

        pros::lcd::set_text(2, "x: " + std::to_string(x));
        pros::lcd::set_text(3, "y: " + std::to_string(y));
        pros::lcd::set_text(4, "theta: " + std::to_string(theta));


		pros::delay(20);
	}
}
