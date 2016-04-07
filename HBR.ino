This is the Arduino sketch for joystick control of the HBR joint.
Using Arduino Uno
two potentiometer inputs on Analog pins using external pull down resistors. 2.5 volts = stop (maybe needs a little tweeking?)
one "run enable" latching switch input that disables all outputs if it goes low. Pin using external pull down. And should be on interupt pin.
four PWM outputs that will drive little 222 transistors to fire the valve solenoids


That's as far as I got. :o
We'll add features as we go... like flow measurements and slow creep control.
