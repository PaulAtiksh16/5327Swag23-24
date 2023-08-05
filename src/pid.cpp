#include "drive.hpp"
#include "main.h"
#include "pros/motors.h"
#include <cmath>

extern pros::Motor front_left_mtr;
extern pros::Motor front_right_mtr;
extern pros::Motor back_left_mtr;
extern pros::Motor back_right_mtr;
extern pros::Motor top_left_mtr;
extern pros::Motor top_right_mtr;

// Omni Wheel diameter is 3.25 inches
const double wheel_radius = 1.625;
const double PI = 3.14159265358979323846;

// MOVE TO HEADER FILE, KEEP IMPLEMENTATION HERE
class PID_controller
{
private:
    // Weights for PID
    double kP;
    double kI;
    double kD;
    // Milliseconds, is the time between the move function is run.
    int time;

    double error = 0;
    double prevError = 0;
    double integral = 0;
    double derivative = 0;

    bool atPosition = false;
public:
    // Constructor, create the weights 
    PID_controller(double P, double I, double D, int T)
    : kP{ P }, kI{ I }, kD{ D }, time{ T }
    {        
    }

    // Update the weights
    void updateProportion(double update) { kP = update; }
    void updateIntegral(double update) { kI = update; }
    void updateDerivative(double update) { kD = update; }

    void updateAtPosition(bool update) { atPosition = update; }

    // Measurements in inches, distance relative to starting position
    // Returns voltage, continuously changes voltage
    double moveTo(double targetDistance, double currentDistance)
    {
        error = targetDistance - currentDistance;

        // Integral is weighted by the amount of iterations, weighted by time 
        // IMPLEMENT CONDITIONAL INTEGRATION (If bot past position and integral says
        // to move forward, set integral to 0 (so it does nothing))
        integral += error * time;

        // Within 0.5 inches of desired position, reset integral to prevent future
        // movement
        if (fabs(error) <= 0.5)
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
        // Clamp the maximum value to 127, ONLY ACCOUNTS FOR FORWARD MOVEMENT
        return (output > 127.0) ? 127.0 : output;
    }
};

double get_distance_in(double encoder_value)
{
    // Value in centidegrees, 100 centidegrees in 1 degree
    return ((encoder_value / 100) / 360) * (2 * PI * wheel_radius);
}


