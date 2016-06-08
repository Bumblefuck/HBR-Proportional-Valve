# HBR-Proportional-Valve

This is an arduino C sketch for controlling proportional valves using the axes of a
potentiometer joystick as inputs.  An enable button / kill switch provides an instant
zero output to the valves.


## Installation

You need the "Universal Inputs" library by Dan Nixon, which can be installed via the Arduino IDE.


## Wiring

| Name           | Type    | Pin number |
| -------------  | -------:| ---------- |
| Joystick X     | Analog  | 0          |
| Joystick Y     | Analog  | 1          |
| Enable button  | Digital | 10         |
| Solenoid X +   | Digital | 3          |
| Solenoid X -   | Digital | 5          |
| Solenoid Y +   | Digital | 6          |
| Solenoid Y -   | Digital | 9          |
