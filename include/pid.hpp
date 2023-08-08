#ifndef _PID_HPP_
#define _PID_HPP_

#ifdef cplusplus
extern "C" {
#endif

#include <stdint.h> // needed for the types declariations
#include <bit>

#ifdef cplusplus
}
#endif

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

public:
    bool atPosition = false;
    // Constructor, create the weights 
    PID_controller(double P, double I, double D, int T=0)
    : kP{ P }, kI{ I }, kD{ D }, time{ T }
    {        
    }

    // Update the weights
    void updateProportion(double update) { kP = update; }
    void updateIntegral(double update) { kI = update; }
    void updateDerivative(double update) { kD = update; }

    // Update and get the position
    void updateAtPosition(bool update) { atPosition = update; }

    int getTime() { return time; }

    // Clamping the Integral to prevent windup
    // Returns true if the integrator should be clamped
    bool clamp(double output, double clamped, double error);

    // Measurements in inches, distance relative to starting position
    // Returns voltage, continuously changes voltage
    double moveTo(double targetDistance, double currentDistance);
};

double get_distance_in(double encoder_value);

#endif //_PID_HPP_
