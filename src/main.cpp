#include "main.h"
#include "lemlib/api.hpp"
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

pros::Motor slapper_mtr(SLAPPER_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);



// tracking wheels
pros::Rotation horizontalEnc(7);
// horizontal tracking wheel. 2.75" diameter, 3.7" offset, back of the robot
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -3.7);

// drivetrain
lemlib::Drivetrain_t drivetrain {&left_side_motors, &right_side_motors, 10.8, lemlib::Omniwheel::NEW_325, 450, 40};

// lateral motion controller
lemlib::ChassisController_t lateralController {70, 130, 1, 100, 3, 500, 20};

// angular motion controller
lemlib::ChassisController_t angularController {1, 10, 1, 100, 3, 500, 20};

// sensors for odometry
lemlib::OdomSensors_t sensors {nullptr, nullptr, nullptr, nullptr, &inertial_sensor};

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize();

    lemlib::Logger::initialize();

    // calibrate sensors
    chassis.calibrate();
    chassis.setPose(lemlib::Pose(0, 0, 0));

    // print odom values to the brain
    pros::Task screenTask([=]() {
        while (true) {
            pros::lcd::print(0, "X: %f", chassis.getPose().x);
            pros::lcd::print(1, "Y: %f", chassis.getPose().y);
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta);
            lemlib::Logger::logOdom(chassis.getPose());
            pros::delay(50);
        }
    });
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
pros::ADIDigitalOut left_wall(LEFT_WALL_PORT, false);
pros::ADIDigitalOut right_wall(RIGHT_WALL_PORT, false);
ASSET(bowl_txt);
void skills(){
    chassis.setPose(45,55, -130);
    pros::Task([=]{
        left_wall.set_value(1);
        pros::delay(900);
        left_wall.set_value(0);
    });
    chassis.turnTo(41, 67, 300, false, false);
    chassis.moveTo(61, 32, 0, 2000, false, false, 40, 0.8, 127);
    chassis.moveTo(25,52, -90, 800, false, true, 40, 0.4);
    chassis.moveTo(-46, 52, -90, 900, false, true, 40, 0.1);
    chassis.moveTo(-56, 0, 180, 2000, false, true, 40, 0.4);
    left_wall.set_value(0);
    right_wall.set_value(0);
    chassis.moveTo(-50, 35, 160, 1000, false, false, 40, 0.01);
    chassis.turnTo(-47,53, 500, false, false);
    chassis.moveTo(-50, 0, 0, 1000, false, false, 40, 0.2);
    left_wall.set_value(1);
    right_wall.set_value(1);
    chassis.moveTo(-50, 15, 45, 1000, false, true, 40, 0.5);
    chassis.moveTo(-35, 20, 115, 500, false, true, 40, 0.4, 127);
    chassis.moveTo(-20, 5, -90, 2000, false, true, 40, 0.4, 127);
    chassis.moveTo(-60, -10, -90, 1300, false, true, 40, 0.4, 127);
    left_wall.set_value(0);
    right_wall.set_value(0);
    chassis.moveTo(-20, 30, 180, 2000, false, false, 40, 0.4, 127);
    chassis.moveTo(-20, 0, 180, 900, false, true, 40, 0.2, 127);
    left_wall.set_value(1);
    right_wall.set_value(1);
    chassis.turnTo(-50, 0, 300, false, false);
    chassis.moveTo(-50, 0, -90, 1400, false, true, 40, 0.2, 127);
    left_wall.set_value(0);
    right_wall.set_value(0);
    chassis.moveTo(-20, 0, -90, 1000, false, false, 40, 0.2, 127);
    chassis.turnTo(50, 0, 300, false, false);
    chassis.moveTo(-20, -37, 0, 1000, false, false, 40, 0.2, 127);
    left_wall.set_value(1);
    right_wall.set_value(1);
    chassis.moveTo(-50, 0, -90, 1000, false, true, 40, 0.2, 127);

    


}
void autonomous() {
    skills();
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
void opcontrol() {}
