#include "main.h"
#include "drive.hpp"
#include "mechs.hpp"
#include "pneumatics.hpp"
#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/rtos.hpp"


//HELPFUL LINKS:
//ascii text generator: https://fsymbols.com/generators/carty/
//PROS API: https://pros.cs.purdue.edu/v5/api/cpp/index.html

/**
CONTROLS:
Joysticks: roll around obviously
L1: flywheel on
L2: flywheel off
R2: kicker macro (out, then back in)
R1: kicker toggle (out first push, back in second push)
X: walls toggle (out first push, back in second push)
*/

//CONTROLLER
pros::Controller master(pros::E_CONTROLLER_MASTER);

//MOTORS
pros::Motor front_left_mtr(FRONT_LEFT_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor front_right_mtr(FRONT_RIGHT_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor back_left_mtr(BACK_LEFT_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor back_right_mtr(BACK_RIGHT_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor top_left_mtr(TOP_LEFT_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor top_right_mtr(TOP_RIGHT_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor left_intake_mtr(LEFT_INTAKE_PORT, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_intake_mtr(RIGHT_INTAKE_PORT, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor flywheel_mtr(FLYWHEEL_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor flywheel_mtr_2(FLYWHEEL_PORT_2, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor left_lift_mtr(LEFT_LIFT_PORT, pros::E_MOTOR_GEAR_RED, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_lift_mtr(RIGHT_LIFT_PORT, pros::E_MOTOR_GEAR_RED, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor conveyor_mtr(CONVEYOR_PORT, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor conveyor_mtr_2(CONVEYOR_PORT_2, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);


//PNEUMATICS
pros::ADIDigitalOut left_wall(LEFT_WALL_PORT, false);
pros::ADIDigitalOut right_wall(RIGHT_WALL_PORT, false);

pros::ADIDigitalOut kicker(KICKER_PORT, false);

pros::ADIDigitalOut grabber(GRABBER_PORT, false);

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
	pros::lcd::initialize();
	pros::lcd::set_text(1, "hi bitchy!");

	pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {
	
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
		// if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
		// 	// run_intake_forward();
		// 	intake();
		// }
		
		// OUTTAKE //
		// if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
		// 	// run_intake_backward();
		// 	outtake();
		// }

		// FLYWHEEL //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			run_flywheel(127);
		}

		// FLYWHEEL OFF //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			run_flywheel(0);
		}

		// EVERYTHING OFF //
		// if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
		// 	everything_off();
		// }

		// INDEX // 
		// if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
		// 	index();
		// }

		// WALLS TOGGLE //
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
			walls_toggle();
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

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			// kick_macro();
			grabber_toggle();
		}



		// DEBUGGING //
		// get_base_watts();
		// get_mech_watts();


		pros::delay(20);
	}
}
