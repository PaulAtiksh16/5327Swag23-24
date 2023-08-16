#ifndef _PID_HPP_
#define _PID_HPP_

#ifdef cplusplus
extern "C" {
#endif

#include <stdint.h> // needed for the types declariations
#include <bit>
#include "main.h"


#ifdef cplusplus
}
#endif

// Motor declarations here
extern pros::Motor front_left_mtr;
extern pros::Motor front_right_mtr;
extern pros::Motor back_left_mtr;
extern pros::Motor back_right_mtr;
extern pros::Motor top_left_mtr;
extern pros::Motor top_right_mtr;

// Function declarations here
double get_distance_in(double encoder_value);

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

    double botRadius;

public:
    bool atPosition = false;
    // Constructor, create the weights 
    PID_controller(double P, double I, double D, int T=0, double r=7.5)
    : kP{ P }, kI{ I }, kD{ D }, time{ T }, botRadius{ r }
    {        
    }

    // Update the weights
    void updateProportion(double update) { kP = update; }
    void updateIntegral(double update) { kI = update; }
    void updateDerivative(double update) { kD = update; }

    // Update and get the position
    void updateAtPosition(bool update) { atPosition = update; }

    int getTime() { return time; }
    double getAveragePosition()
        { return front_left_mtr.get_position() + front_right_mtr.get_position() + back_left_mtr.get_position() + back_right_mtr.get_position() / 4; }

    // Clamping the Integral to prevent windup
    // Returns true if the integrator should be clamped
    bool clamp(double output, double clamped, double error);

    // Measurements in inches, distance relative to starting position
    // Returns voltage, continuously changes voltage
    double moveTo(double targetDistance, double currentDistance);
    double turn(double targetAngle, double currentAngle);
    double simpleMove(double targetDistance, double currentDistance);

    void resetWeights()
    {
        kP = 0;
        kI = 0;
        kD = 0;
        atPosition = false;
    }
    void updateWeights(double P=0.0, double I=0.0, double D=0.0)
    {
        kP = P;
        kI = I;
        kD = D;
    }
};

double pidMove(double target, double current);
double get_distance_in(double encoder_value);

void move_lateral_pid(PID_controller piss, double targetDistance);
void move_turn_pid(PID_controller piss, double targetAngle);

#endif //_PID_HPP_
