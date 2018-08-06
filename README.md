# HBR-Proportional-Valve
This is an arduino C sketch for controlling a proportional valve with potentiometer joy stick.

## Installation

You need the "Universal Inputs" library by Dan Nixon, which can be installed via the Arduino IDE.


## Wiring

| Name           | Type    | Pin number |
| -------------  | -------:| ---------- |
| Joystick X     | Analog  | A0         |
| Joystick Y     | Analog  | A1         |
| Ramp Timer Pot | Analog  | A2         | 
| Enable button  | Digital | 10         |
| Up Button      | Digital | 13         | 
| Down Button    | Digital | 12         | 
| Left Button    | Digital | 8          | 
| Right Button   | Digital | 7          |
| Solenoid X +   | Digital | 3 PWM      |
| Solenoid X -   | Digital | 5 PWM      |
| Solenoid Y +   | Digital | 6 PWM      |
| Solenoid Y -   | Digital | 9 PWM      |

