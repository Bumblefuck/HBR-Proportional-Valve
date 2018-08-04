#include <ArduinoJoystick.h>
#include <LinearTransform.h>
#include <ArduinoButton.h>

//Define all the pins so the preprocessor subs them into the bytecode, saves memory
#define PIN_JOY_X   0
#define PIN_JOY_Y   1
#define PIN_EN_BTN  10
#define PIN_VALVE_X_POS 3
#define PIN_VALVE_X_NEG 5
#define PIN_VALVE_Y_POS 6
#define PIN_VALVE_Y_NEG 7

//Define array axis
#define AXIS_X 0
#define AXIS_Y 1


/**
 * Pin structure to capture each pin valve pin position
 **/
typedef struct valve_pin {
  int pos;
  int neg;
} ValvePin;

// Set up an array of joystick axes
// This is equivalent to "joystick[axisX] = ArduinoJoystick(0, PIN_JOY_X)"
ArduinoJoystick joystick[] = {
  ArduinoJoystick(0, PIN_JOY_X),
  ArduinoJoystick(1, PIN_JOY_Y)
};

// Set up the button which will control start/stop
ArduinoButton enableButton(2, PIN_EN_BTN);

/**
 * Set up the valves as an array of structs
 * ValvePin structs can be initialized like arrays as such: 
 *  ValvePin myVlave = {2, 5}; 
 * So this is a 2 element array of 2 element structs, defined in shorthand
 **/ 
ValvePin valve[] = {
  { PIN_VALVE_X_POS, PIN_VALVE_X_NEG },
  { PIN_VALVE_Y_POS, PIN_VALVE_Y_NEG },
};


/**
 * Setup the joystick axis
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
  LinearTransform* joystickPotTransform = new LinearTransform(-128, 126, -512, 512);
  joystick[AXIS_X].setTransformation(joystickPotTransform);
  joystick[AXIS_Y].setTransformation(joystickPotTransform);

}

/**
 * Initialize the valve axis
 **/
void init_valve_axes() {
  for (int axis = AXIS_X; axis <= AXIS_Y; ++axis) {
	pinMode(valve[axis].pos, OUTPUT);
	pinMode(valve[axis].neg, OUTPUT);
  }
}


/**
 * Set the positive or negative valve psoition
 * 
 * @param axis The axis for this valve
 * @param valve_position The position for the valve
 **/
void set_valve(int axis, int valve_position) {
  // Set the positive valve position to zero if the desired position is 0 (or less), otherwise the desired value
  analogWrite(valve[axis].pos, valve_position <= 0 ? 0 : valve_position);
  // Set the negative valve position to zero if the desired position is 0 (or more), otherwise the absolute desired value
  analogWrite(valve[axis].neg, valve_position >= 0 ? 0 : abs(valve_position));
}


/**
 * Print some positional data
 * @param x_val The x position value
 * @param y_val The y position value 
 * @param is_enabled Weather its enabled or not
 **/
void print_positional_data(int x_val, int y_val, bool is_enabled) {
  Serial.print("Joystick Position - X: ");
  Serial.print(x_val);
  Serial.print("\t Y: ");
  Serial.print(y_val);
  Serial.print("\t");
  Serial.println(is_enabled ? "enabled" : "DISABLED");
}

/**
 * Program setup function
 * setup the inputs and outputs
 * set everything to 0
 **/
void setup()
{
  Serial.begin(9600);

  // initial setup of inputs and outputs
  init_joystick_axes();
  init_valve_axes();

  // set all the valves to "off" -- the 0 position
  for (int axis = AXIS_X; axis <= AXIS_Y; ++axis) {
	set_valve(axis, 0);
  }
}

/**
 * The main loop of the program
 * read the joysticks
 * set the valves 
 * print the data that has changed
 **/
void loop()
{
   bool isEnabled = false;   // indicates the enable button position
   bool gotJoystick = false; // indicates whether an updated joystick position was received

   //check the state of the enable button
   enableButton.poll();
   isEnabled = enableButton.isActive();

  // read data from each joystick axis and apply it to the valve.
  // if the data is new, make a note of it
  for (int axis = AXIS_X; axis <= AXIS_Y; ++axis) {
	gotJoystick = gotJoystick || joystick[axis].poll();

	// read the joystick value and constrain it, but only send zero unless the enable button is pressed
	int stickPosition = constrain(joystick[axis].getValue(), -255, 255);
	set_valve(axis, isEnabled ? stickPosition : 0);
  }

  // limit logging by only printing stuff out if we received a change
  if (gotJoystick) {
	print_positional_data(joystick[AXIS_X].getValue(), joystick[AXIS_Y].getValue(), isEnabled);
  }
}
