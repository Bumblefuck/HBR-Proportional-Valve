#ifndef __DEFINES_H
#define __DEFINES_H

// | Name           | Type    | Pin number |
// | -------------  | -------:| ---------- |
// | Joystick X     | Analog  | A0         |
// | Joystick Y     | Analog  | A1         |
// | Ramp Timer Pot | Analog  | A2         | 
// | Enable button  | Digital | 10         |
// | Up Button      | Digital | 13         | 
// | Down Button    | Digital | 12         | 
// | Left Button    | Digital | 8          | 
// | Right Button   | Digital | 7          |
// | Solenoid X +   | Digital | 3 PWM      |
// | Solenoid X -   | Digital | 5 PWM      |
// | Solenoid Y +   | Digital | 6 PWM      |
// | Solenoid Y -   | Digital | 9 PWM      |

//Define physical pins here
#define PIN_JOY_X   0 //A0
#define PIN_JOY_Y   1 //A1
#define RAMP_POT    2 //A2
#define PIN_EN_BTN  10 //DIGITAL
#define UP_BTN      13  //DIGITAL
#define DWN_BTN     12 //DIGITAL
#define LEFT_BTN    8 //DIGITAL
#define RIGHT_BTN   7 //DIGITAL
#define PIN_VALVE_X_POS 3 //PWM
#define PIN_VALVE_X_NEG 5 //PWM
#define PIN_VALVE_Y_POS 6 //PWM
#define PIN_VALVE_Y_NEG 9 //PWM

// //Define array axis
 #define AXIS_X 0
 #define AXIS_Y 1
#endif