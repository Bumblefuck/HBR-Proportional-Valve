#include <ArduinoJoystick.h>
#include <LinearTransform.h>

ArduinoJoystick joystickX(0, 0);
ArduinoJoystick joystickY(1, 1);

LinearTransform transform(-128, 126, -512, 512);

#define VALVE_X_POS 3
#define VALVE_X_NEG 5
#define VALVE_Y_POS 6
#define VALVE_Y_NEG 9

int pwmPins[] = {VALVE_X_POS, VALVE_X_NEG, VALVE_Y_POS, VALVE_Y_NEG};

void setup()
{
  Serial.begin(9600);
  
  joystickX.setPoints(390, 520, 640);
  joystickY.setPoints(390, 520, 640);
  
  joystickX.setDeadbands(5, 5, 5);
  joystickY.setDeadbands(5, 5, 5);
  
  joystickX.setThreshold(5);
  joystickY.setThreshold(5);
  
  joystickX.setTransformation(&transform);
  joystickY.setTransformation(&transform);

  for (int i = 0; i < 4; i++)
  {
    pinMode(pwmPins[i], OUTPUT);
    analogWrite(pwmPins[i], 0);
  }
}

void loop()
{
  if(joystickX.poll())
  {
    uint16_t value = joystickX.getValue();
    Serial.print("X joystick: ");
    Serial.print(value);

    if (value > 0)
    {
      analogWrite(VALVE_X_POS, value);
      analogWrite(VALVE_X_NEG, 0);
    }
    else if (value < 0)
    {
      analogWrite(VALVE_X_POS, 0);
      analogWrite(VALVE_X_NEG, value);
    }
  }
  
  if(joystickY.poll())
  {
    uint16_t value = joystickY.getValue();
    Serial.print("Y joystick: ");
    Serial.print(value);

    if (value > 0)
    {
      analogWrite(VALVE_Y_POS, value);
      analogWrite(VALVE_Y_NEG, 0);
    }
    else if (value < 0)
    {
      analogWrite(VALVE_Y_POS, 0);
      analogWrite(VALVE_Y_NEG, value);
    }
  }
}
