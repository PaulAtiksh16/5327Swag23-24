#include "drive.hpp"
#include "main.h"
#include "pros/motors.h"
#include "pid.hpp"

extern pros::Motor front_left_mtr;
extern pros::Motor front_right_mtr;
extern pros::Motor back_left_mtr;
extern pros::Motor back_right_mtr;
extern pros::Motor top_left_mtr;
extern pros::Motor top_right_mtr;

// Omni Wheel diameter is 3.25 inches
const double wheel_radius = 1.625;
const double PI = 3.14159265358979323846;


// Clamping the Integral to prevent windup
// Returns true if the integrator should be clamped
bool PID_controller::clamp(double output, double clamped, double error)
{
    /*
    No saturation took place if the clamped output is the same as the raw output

    Looking to see if the output is trying to make the error "more" of what it is
        i.e. if there's positive error (bot behind position) and positive output 
        (bot moving towards position), then we should clamp the integrator to slow
        down.
    */
    if ((output == clamped) && (std::signbit(output) == std::signbit(error)))
    {
        return true;
    }

    return false;
}
// Measurements in inches, distance relative to starting position
// Returns voltage, continuously changes voltage
double PID_controller::moveTo(double targetDistance, double currentDistance)
{
    error = targetDistance - currentDistance;

    // Integral is weighted by the amount of iterations, weighted by time 
    // IMPLEMENT CONDITIONAL INTEGRATION (If bot past position and integral says
    // to move forward, set integral to 0 (so it does nothing))
    integral += error * time;

    // Within 0.5 inches of desired position, reset integral to prevent future
    // movement
    if (error <= 1.0)
    {
        integral = 0;
        updateAtPosition(true);
    }

    // Takes how much error has changed since last iteration, and divide by 
    // time to determine how fast or slow it moves between iterations.
    
    derivative = (error - prevError) / time;
    prevError = error;
    

    // Proportion part is propWeight times error (gets smaller as 
    // bot gets closer to target distance)
    double output = kP * error + kI * integral + kD * derivative;
    // Clamp the maximum value to 127, ONLY ACCOUNTS FOR FORWARD SATURATION
    // SET LOWER TO 127 IN THE FUTURE TO ACCOUNT FOR MOTOR BURNOUT, maybe make maxVoltage attribute?
    double clampedOutput = (output > 12000.0) ? 12000.0 : output;

    // Turn off integrator
    /*
    When output not saturated, turn on again
    When moving forwards but need to go backwards, turn on again
    */
    
    if (clamp(output, clampedOutput, error))
        integral = 0;
    
    return clampedOutput;
}

double PID_controller::turn(double targetAngle, double currentAngle)
{
    // Convert target angle to radians, then to the distance the wheels need to travel
    // to reach the target angle
    double desired_value_in_radians = targetAngle * (PI / 180);
    double arc_length = botRadius * desired_value_in_radians;
    arc_length = 360 * (arc_length / (3.25 * PI));


    double average_position = PID_controller::getAveragePosition();
    double error = (arc_length - average_position);

    integral += error * time;
    if (error <= 1.0)
    {
        integral = 0;
        updateAtPosition(true);
    }
    
    derivative = (error - prevError) / time;
    prevError = error;

    double output = kP * error + kI * integral + kD * derivative;
    // Clamp the maximum value to 127, ONLY ACCOUNTS FOR FORWARD SATURATION
    // SET LOWER TO 127 IN THE FUTURE TO ACCOUNT FOR MOTOR BURNOUT, maybe make maxVoltage attribute?
    double clampedOutput = (output > 12000.0) ? 12000.0 : output;

    // Turn off integrator
    /*
    When output not saturated, turn on again
    When moving forwards but need to go backwards, turn on again
    */
    
    if (clamp(output, clampedOutput, error))
        integral = 0;
    
    return clampedOutput;
}

double PID_controller::simpleMove(double targetDistance, double currentDistance)
{
    double error = targetDistance-currentDistance;
    return 24*error;
}

double get_distance_in(double encoder_value)
{
    // Value in degrees
    return (encoder_value / 360.0) * (2 * PI * wheel_radius);
}



double pidMove(double target, double current)
{
    double error = target-current;
    return 24*error;
}



void move_lateral_pid(PID_controller &piss, double targetDistance) {
    //reset encoders
    base_tare_position();
    
    // Reset PID class
	piss.resetWeights();
	piss.updateWeights(1300.0, 0.02, 6);
    
    while (piss.atPosition == false)
	{
		// Get distance moved, lmk if u want to change input to something else other than encoder units
		double distance = get_distance_in(front_left_mtr.get_position());
		base_move_voltage(piss.moveTo(targetDistance, distance));
		pros::delay(5);		
	}
    base_move_voltage(0);
}


void move_turn_pid(PID_controller &piss, double targetAngle) {
    //reset encoders
    base_tare_position();
    
    // Reset PID class
    piss.resetWeights();
    piss.updateWeights(1300, 0, 0);
    
    while (piss.atPosition == false) 
	{
		double distance = get_distance_in(piss.getAveragePosition());
		double voltage = piss.turn(targetAngle, distance);
		left_move_voltage(voltage);
		right_move_voltage(-voltage);
		pros::delay(5);
	}
    base_move_voltage(0);
}