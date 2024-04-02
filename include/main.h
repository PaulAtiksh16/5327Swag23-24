/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

//SMART PORTS BELOW
#define FRONT_LEFT_PORT 8
#define MID_LEFT_PORT 10
#define BACK_LEFT_PORT 9

#define FRONT_RIGHT_PORT 11
#define MID_RIGHT_PORT 3
#define BACK_RIGHT_PORT 2

#define INTAKE_PORT 17

#define SLAPPER_PORT 13

#define INERTIAL_PORT 16



//ADI TRIPORTS BELOW
#define LEFT_WALL_PORT 7
#define RIGHT_WALL_PORT 9

#define BACK_WALLS_PORT 8

#define LEFT_HANG_PORT 2
#define RIGHT_HANG_PORT 4

#define INTAKE_LINE_TRACKER 1

#define LOCK_PORT 5

#define BLOCKER_PORT 5


//UNUSED PORTS BELOW
#define FLYWHEEL_PORT 1

#define LEFT_INTAKE_PORT 3
#define RIGHT_INTAKE_PORT 4

#define CONVEYOR_PORT 5
#define CONVEYOR_PORT_2 3

#define LEFT_LIFT_PORT 12
#define RIGHT_LIFT_PORT 14
#define PUNCHER_PORT 17

#define LIFT_PORT 5


#define FLYWHEEL_PORT_2 19

//UNUSED TRIPORTS BELOW
#define KICKER_PORT 3

#define GRABBER_PORT 1

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convenient for most student programmers.
 */
#define PROS_USE_SIMPLE_NAMES

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
#define PROS_USE_LITERALS

#include "api.h"

/**
 * You should add more #includes here
 */
//#include "okapi/api.hpp"
//#include "pros/api_legacy.h"

/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
// using namespace pros;
// using namespace pros::literals;
// using namespace okapi;

/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */
#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream>
#endif

#endif  // _PROS_MAIN_H_
