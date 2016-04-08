/* Read Joystick
 * ------------
 *
 * Reads two analog pins that are supposed to be
 * connected to a joystick made of two potentiometers
 *
 * We send three bytes back to the comp: one header and two
 * with data as signed bytes, this will take the form:
 *     Jxy\r\n
 *
 * x and y are integers and sent in ASCII
 *
 * http://www.0j0.org | http://arduino.berlios.de
 * copyleft 2005 DojoDave for DojoCorp
 */

int potPin = 0;
int rampTime = 0;

static const int RAMP_UP = 0;
static const int RAMP_DOWN = 1;

// These arrays contain the configuration for the buttons.
// buttonPins maps the buttons to the pins
// buttonValues holds the values for the actual pins. I'm assuming INPUT_PULLUP
// on the pin mode which means that HIGH is off
// buttonOutputPins maps the buttons to which analog output pin should be used
const int buttons = 4;
int buttonPins[buttons] = {1, 2, 3, 4};
int buttonValues[buttons] = {HIGH, HIGH, HIGH, HIGH};
int buttonOutputPins[buttons] = {6, 9, 10, 11};
int buttonRampValues[buttons] = {0, 0, 0, 0};
unsigned long buttonRampTimes[buttons] = {0, 0, 0, 0};

// Ramp Time bounds in milliseconds
// The actual ramp time will be calculated in a range from minRampTime
// to maxRampTime based on the potentiometer
int minRampTime = 100;
int maxRampTime = 3000;

void setup() {
  Serial.begin(9600);

  // Set all the button input pins to be used in INPUT_PULLUP mode
  for(int i = 0; i < buttons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

int mapPotToRamp(int potValue) {
  // The analog input returns a value from 0 to 1023.
  // We need to map this to a time value
  int rampTime = (potValue * maxRampTime / 1023);

  // This allows us to set a minimum ramp time to prevent it from ever being "0"
  if(rampTime < minRampTime) {
    rampTime = minRampTime;
  }
}

// Map the ramp time from somewhere between 0 and 255
int mapRampToValue(int currentValue, int direction, unsigned long time, int rampTime) {
  int newValue = 0;

  // Map the new value based on the current value and how much time has occurred.
  if(direction == RAMP_UP) {
    newValue = currentValue + ((time / rampTime) * 255);
  } else {
    newValue = currentValue - ((time / rampTime) * 255);
  }

  // Prevent values higher than 255
  if(newValue > 255) {
    newValue = 255;
  }

  // Prevent values lower than 0
  if(newValue < 0) {
    newValue = 0;
  }

  return newValue;
}

void ramp(int button, int direction, int rampTime) {
  // Calculate how long it's been since the last ramp change
  unsigned long current = millis();
  unsigned long difference = current - buttonRampTimes[button];

  int outputValue = mapRampToValue(buttonRampValues[button], direction, current - difference, rampTime);
  analogWrite(buttonOutputPins[button], outputValue);
}

void loop() {
  // reads the value of the variable resistor
  int potValue = analogRead(potPin);

  rampTime = mapPotToRamp(potValue);

  // Read all of the button values
  for(int i = 0; i < buttons; i++) {
    buttonValues[i] = digitalRead(buttonPins[i]);

    int direction;

    // Remember, we're doing PULLUP so this means if the button is pushed
    if(buttonValues[i] == LOW) {
      direction = RAMP_UP;
    } else {
      direction = RAMP_DOWN;
    }

    ramp(i, direction, rampTime);
  }

  // Give us a 100 millisecond delay to let the ramp do its thing
  delay(100);
}
