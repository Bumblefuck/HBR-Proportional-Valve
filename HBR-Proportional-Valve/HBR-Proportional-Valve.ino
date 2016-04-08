#include <ArduinoJoystick.h>
#include <LinearTransform.h>
#include <ArduinoButton.h>

//declare the pins for the joystick X and Y axis
const int16_t JOY_X_PIN = 0;
const int16_t JOY_Y_PIN = 1;

//declare the enable button pin
const int16_t EN_BTN_PIN = 10;

//declare the output pins (must be PWM capable) for solenoid control
const int16_t VALVE_X_POS = 3;
const int16_t VALVE_X_NEG = 5;
const int16_t VALVE_Y_POS = 6;
const int16_t VALVE_Y_NEG = 9;


//setup the two joystick axis
ArduinoJoystick joystickX(0, 0);
ArduinoJoystick joystickY(1, 1);

//setup the button to start/stop
ArduinoButton enableButton(2, 10);

//setup the transform from the input pots to the output PWM signals for the valves
LinearTransform transform(-128, 126, -512, 512);

int16_t xValue = 0;
int16_t yValue = 0;


int pwmPins[] = {VALVE_X_POS, VALVE_X_NEG, VALVE_Y_POS, VALVE_Y_NEG};

void setup()
{
  Serial.begin(9600);
  
  //calibrate for joystick found here: http://uk.rs-online.com/web/p/products/8430838/
  //set Low/Center/High positions
  joystickX.setPoints(390, 520, 640);
  joystickY.setPoints(390, 520, 640);

  //set the deadbands
  joystickX.setDeadbands(5, 5, 5);
  joystickY.setDeadbands(5, 5, 5);

  //set the minimum change to trigger an event
  joystickX.setThreshold(5);
  joystickY.setThreshold(5);

  //set the transform from input values to output values
  joystickX.setTransformation(&transform);
  joystickY.setTransformation(&transform);

  xValue = joystickX.getValue();
  yValue = joystickY.getValue();


  //set all the pins as outputs and initialize to off
  for (int i = 0; i < 4; i++)
  {
    pinMode(pwmPins[i], OUTPUT);
    analogWrite(pwmPins[i], 0);
  }
}

void loop()
{
  enableButton.poll();  //check for changes in the button enable state
  
  //check for updates in the joystick X position
  if(joystickX.poll())
  {
    //grab the current value and print it out for debugging
    xValue = constrain(joystickX.getValue(), -255, 255);
    printPositionData();

    //only allow the system to move if the button is active
    if (enableButton.isActive())
    {
      if (xValue > 0)  
      {
        analogWrite(VALVE_X_POS, abs(xValue));
        analogWrite(VALVE_X_NEG, 0);
      }
      else if (xValue < 0)
      {
        analogWrite(VALVE_X_POS, 0);
        analogWrite(VALVE_X_NEG, abs(xValue));
      }
    }

    //otherwise turn off both valves for X axis
    else
    {
      analogWrite(VALVE_X_POS, 0);
      analogWrite(VALVE_X_NEG, 0);
    }
  }
  


  //check for updates in the joystick Y position
  if(joystickY.poll())
  {
    //grab the current value and print it out for debugging
    yValue = constrain(joystickY.getValue(), -255, 255);
    printPositionData();

    //only allow the system to move if the button is active
    if (enableButton.isActive())
    {
      if (yValue > 0)
      {
        analogWrite(VALVE_Y_POS, abs(yValue));
        analogWrite(VALVE_Y_NEG, 0);
      }
      else if (yValue < 0)
      {
        analogWrite(VALVE_Y_POS, 0);
        analogWrite(VALVE_Y_NEG, abs(yValue));
      }
    }

    //otherwise turn off both valves for Y axis
    else
    {
      analogWrite(VALVE_Y_POS, 0);
      analogWrite(VALVE_Y_NEG, 0);
    }
  }

}


void printPositionData(){
  Serial.print("Joystick Position - X: ");
  Serial.print(xValue);
  Serial.print("\t Y: ");
  Serial.println(yValue);
}









