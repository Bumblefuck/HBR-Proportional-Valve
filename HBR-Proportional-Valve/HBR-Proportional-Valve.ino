#include <ArduinoJoystick.h>
#include <LinearTransform.h>
#include <ArduinoButton.h>

// declare the pins for the inputs and outputs
const int PIN_JOY_X       = 0;
const int PIN_JOY_Y       = 1;
const int PIN_EN_BTN      = 10;
const int PIN_VALVE_X_POS = 3;
const int PIN_VALVE_X_NEG = 5;
const int PIN_VALVE_Y_POS = 6;
const int PIN_VALVE_Y_NEG = 9;

// Valves have a positive and a negative actuation - we need a pin for each.
// This structure captures that.
struct ValvePin {
  int pos;
  int neg;
};

// Indexes for joystick-related arrays
const int axisX = 0;
const int axisY = 1;

// Set up an array of joystick axes
// This is equivalent to "joystick[axisX] = ArduinoJoystick(0, PIN_JOY_X)"
ArduinoJoystick joystick[] = {
  ArduinoJoystick(0, PIN_JOY_X),
  ArduinoJoystick(1, PIN_JOY_Y)
};

// Set up the button which will control start/stop
ArduinoButton enableButton(2, PIN_EN_BTN);
bool lastButtonState = false;

// Set up the valves as an array of structs.
// ValvePin structs can be initialized like arrays, as such:
//   ValvePin myValve = { 2, 5 };
// So this is a 2-element array of 2-element structs, defined in shorthand:
ValvePin valve[] = {
  { PIN_VALVE_X_POS, PIN_VALVE_X_NEG },
  { PIN_VALVE_Y_POS, PIN_VALVE_Y_NEG },
};


// Perform all initial setup for the joystick axes
void initJoystickAxes() {
  // X axis
  joystick[axisX].setPoints(390, 520, 640); // Low/Center/High positions
  joystick[axisX].setDeadbands(5, 5, 5);
  joystick[axisX].setThreshold(5);          // minimum change to trigger an event

  // Y axis
  joystick[axisY].setPoints(390, 520, 640); // Low/Center/High positions
  joystick[axisY].setDeadbands(5, 5, 5);
  joystick[axisY].setThreshold(5);          // minimum change to trigger an event

  // set up the transform from the input pots to the output PWM signals for the valves
  LinearTransform* joystickPotTransform = new LinearTransform(-128, 126, -512, 512);
  joystick[axisX].setTransformation(joystickPotTransform);
  joystick[axisY].setTransformation(joystickPotTransform);

}

// Perform all the inital setup for the valve pins
void initValveAxes() {
  for (int axis = axisX; axis <= axisY; ++axis) {
    pinMode(valve[axis].pos, OUTPUT);
    pinMode(valve[axis].neg, OUTPUT);
  }
}


// Set the positive or negative valve position
// axis - the array index (axis) for this valve
// valvePosition - the desired position
void setValve(int axis, int valvePosition) {
  // Set the positive valve position to zero if the desired position is 0 (or less), otherwise the desired value
  analogWrite(valve[axis].pos, valvePosition <= 0 ? 0 : valvePosition);
  // Set the negative valve position to zero if the desired position is 0 (or more), otherwise the absolute desired value
  analogWrite(valve[axis].neg, valvePosition >= 0 ? 0 : abs(valvePosition));
}


// print some debugging output about the joystick positions
void printPositionData(int xValue, int yValue, bool isEnabled) {
  Serial.print("Joystick Position - X: ");
  Serial.print(xValue);
  Serial.print("\t Y: ");
  Serial.print(yValue);
  Serial.print("\t");
  Serial.println(isEnabled ? "enabled" : "DISABLED");
}


void setup()
{
  Serial.begin(9600);

  // initial setup of inputs and outputs
  initJoystickAxes();
  initValveAxes();

  // set all the valves to "off" -- the 0 position
  for (int axis = axisX; axis <= axisY; ++axis) {
    setValve(axis, 0);
  }
}


void loop()
{
   bool isEnabled = false;   // indicates the enable button position
   bool gotButton = false;   // indicates whether an updated button position was received
   bool gotJoystick = false; // indicates whether an updated joystick position was received

   //check the state of the enable button and see if it's changed
   enableButton.poll();
   isEnabled = enableButton.isActive();
   gotButton = isEnabled != lastButtonState;
   lastButtonState = isEnabled;

  // read data from each joystick axis and apply it to the valve.
  // if the data is new, make a note of it
  for (int axis = axisX; axis <= axisY; ++axis) {
    gotJoystick = gotJoystick || joystick[axis].poll();

    // read the joystick value and constrain it
    int stickPosition = constrain(joystick[axis].getValue(), -255, 255);
    // set valve position to zero unless enable button is pressed
    setValve(axis, isEnabled ? stickPosition : 0);
  }

  // limit logging by only printing stuff out if we received a change
  if (gotButton || gotJoystick) {
    printPositionData(joystick[axisX].getValue(), joystick[axisY].getValue(), isEnabled);
  }
}
