#include "main.h"
#include "drive.hpp"
#include "mechs.hpp"
#include "pid.hpp"
#include "pneumatics.hpp"
#include "pros/adi.hpp"
#include "pros/apix.h"
#include "pros/rtos.hpp"
#include "autonomous.hpp"



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

pros::ADIDigitalOut locking(LOCK_PORT, false);
pros::ADIDigitalOut blocker(BLOCKER_PORT, false);

pros::ADIDigitalOut kicker(KICKER_PORT, false);

pros::ADIDigitalOut grabber(GRABBER_PORT, false);



// Chassis constructor
Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {-13, -14, -11}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{18, 19, 20}

  // IMU Port
  ,4

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,3.25

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,1.666

  // Uncomment if using tracking wheels
  /*
  // Left Tracking Wheel Ports (negative port will reverse it!)
  // ,{1, 2} // 3 wire encoder
  // ,8 // Rotation sensor

  // Right Tracking Wheel Ports (negative port will reverse it!)
  // ,{-3, -4} // 3 wire encoder
  // ,-9 // Rotation sensor
  */

  // Uncomment if tracking wheels are plugged into a 3 wire expander
  // 3 Wire Port Expander Smart Port
  // ,1
);



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
	ez::print_ez_template();
  
	pros::delay(500); // Stop the user from doing anything while legacy ports configure.

	// Configure your chassis controls
	chassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
	chassis.set_active_brake(0); // Sets the active brake kP. We recommend 0.1.
	chassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
	default_constants(); // Set the drive to your own constants from autons.cpp!
	exit_condition_defaults(); // Set the exit conditions to your own constants from autons.cpp!

	// These are already defaulted to these buttons, but you can change the left/right curve buttons here!
	// chassis.set_left_curve_buttons (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used. 
	// chassis.set_right_curve_buttons(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

	// Autonomous Selector using LLEMU
	ez::as::auton_selector.add_autons({
		Auton("Example Drive\n\nDrive forward and come back.", drive_example),
		Auton("Example Turn\n\nTurn 3 times.", turn_example),
		Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
		Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
		Auton("Swing Example\n\nSwing, drive, swing.", swing_example),
		Auton("Combine all 3 movements", combining_movements),
		Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
	});

	// Initialize chassis and auton selector
	chassis.initialize();
	ez::as::initialize();
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
	// base_move_relative(-200, 100);
	// pros::delay(500);

	// base_move_relative(-2000, 1000);
	// pros::delay(1000);
	// base_move_relative(1000, 500);

	drive_example();
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

	// unlock lift at start of driver control
	lock_toggle();

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
			// pros::delay(500);
			// move_lift_end();
			pros::delay(250);
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


		pros::delay(20);
	}
}
