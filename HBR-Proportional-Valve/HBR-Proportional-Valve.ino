/**
* Let the bumblefuckery commence.
*
* Contributors: 
* -----------------------------
* Demetry Romanowski 
* demetryromanowski@gmail.com 
* Ontario, Canada
* -----------------------------
*/

#include <Universal_Inputs/ArduinoJoystick.h>
#include <Universal_Inputs/LinearTransform.h>
#include <Universal_Inputs/ArduinoButton.h>

#include "defines.h"

/**
 * Valve pins struct typedef'd for easier usage
 */
typedef struct valve_pin {
  int pos;
  int neg;
} ValvePin;

/**
 * Set up joystick axis
 */
ArduinoJoystick joystick[] = {
  ArduinoJoystick(0, PIN_JOY_X),
  ArduinoJoystick(1, PIN_JOY_Y)
};

// Set up the button which will control start/stop
ArduinoButton *en_btn = new ArduinoButton(2, PIN_EN_BTN);
ArduinoButton *up_btn = new ArduinoButton(2, UP_BTN); 
ArduinoButton *down_btn = new ArduinoButton(2, DWN_BTN);
ArduinoButton *left_btn = new ArduinoButton(2, LEFT_BTN);

//ArduinoButton *up_btn();

/************************************************************/
/**
 * Set up the valves as an array of structs. 
 * ValvePins can be initialized like arrays as such: 
 *		ValvePin myValve = { 2, 5 } 
 * 
 * So this is a two-element array of two element structs, defined in shorthand
 */
ValvePin valve[] {
  { PIN_VALVE_X_POS, PIN_VALVE_X_NEG },
  { PIN_VALVE_Y_POS, PIN_VALVE_Y_NEG },
};

/************************************************************/
/**
 * Init the joystick axes
 **/
void init_joystick_axes() {
	// X axis
	joystick[AXIS_X].setPoints(390, 520, 640); // Low/Center/High positions
	joystick[AXIS_X].setDeadbands(5, 5, 5);
	joystick[AXIS_X].setThreshold(5);          // minimum change to trigger an event

	// Y axis
	joystick[AXIS_Y].setPoints(390, 520, 640); // Low/Center/High positions
	joystick[AXIS_Y].setDeadbands(5, 5, 5);
	joystick[AXIS_Y].setThreshold(5);          // minimum change to trigger an event

	// set up the transform from the input pots to the output PWM signals for the valves
	LinearTransform* joystick_pot_transform = new LinearTransform(-128, 126, -512, 512);
	joystick[AXIS_X].setTransformation(joystick_pot_transform);
	joystick[AXIS_Y].setTransformation(joystick_pot_transform);

}

/************************************************************/
/**
 * Init the valve axes
 */
void init_valve_axis() {
	ValvePin *valve = &*valve; 

	while (valve != nullptr) {
		pinMode(valve->neg, OUTPUT);
		pinMode(valve->pos, OUTPUT);
		valve++;
	}

	/*for (int axis = AXIS_X; axis <= AXIS_Y; ++axis) {
		pinMode(valve[axis].pos, OUTPUT);
		pinMode(valve[axis].neg, OUTPUT);
	}*/
}
/************************************************************/
/**
 * Set the positive or negative valve position
 * @param axis The axis for this valve
 * @param valve_position The desired position
 */
void set_valve(int axis, int valve_position) {
  // Set the positive valve position to zero if the desired position is 0 (or less), otherwise the desired value
  analogWrite(valve[axis].pos, valve_position <= 0 ? 0 : valve_position);
  // Set the negative valve position to zero if the desired position is 0 (or more), otherwise the absolute desired value
  analogWrite(valve[axis].neg, valve_position >= 0 ? 0 : abs(valve_position));
}

/************************************************************/
/**
 * Print the positional data
 * @param x_value The x value
 * @param y_value The y value
 * @param is_enabled If its enabled
 */
void print_position_data(int x_value, int y_value, bool is_enabled) {
	Serial.print("Joystick Position - X: ");
	Serial.print(x_value);
	Serial.print("\t Y: ");
	Serial.print(y_value);
	Serial.print("\t");
	Serial.println(is_enabled ? "enabled" : "DISABLED");
}

/************************************************************/
/**
 * Setup function, runs once only
 */
void setup()
{
	//Set the PWM clock speed prescaler. 
	TCCR1B = TCCR1B & B11111000 | B00000001;

#ifdef __DEBUG
	//Wait for serial connection to become a available
	while (!Serial.available)
		;
#endif

	Serial.begin(9600);

	// initial setup of inputs and outputs
	init_joystick_axes();
	init_valve_axis();
					
	// set all the valves to "off" -- the 0 position
	for (int axis = AXIS_X; axis <= AXIS_Y; ++axis) {
		set_valve(axis, 0);
	}
}

/************************************************************/
/**
 * Loop function runs constantly
 */
void loop()
{
	bool is_enabled = false;   // indicates the enable button position
	bool got_joystick = false; // indicates whether an updated joystick position was received

	//check the state of the enable button
	en_btn->poll();
	is_enabled = en_btn->isActive();

	// read data from each joystick axis and apply it to the valve.
	// if the data is new, make a note of it
	for (int axis = AXIS_X; axis <= AXIS_Y; ++axis) {
		got_joystick = got_joystick || joystick[axis].poll();

		// read the joystick value and constrain it, but only send zero unless the enable button is pressed
		const int stick_position = constrain(joystick[axis].getValue(), -255, 255);
		set_valve(axis, is_enabled ? stick_position : 0);
	}

	// limit logging by only printing stuff out if we received a change
	if (got_joystick) {
		print_position_data(joystick[AXIS_X].getValue(), joystick[AXIS_Y].getValue(), is_enabled);
	}
}
