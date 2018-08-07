/* Joystick
https://www.adafruit.com/product/2765 (mini single finger/thumb)
https://www.adafruit.com/product/3246 (breakout board for mini not required but easier to work with)
https://www.adafruit.com/product/3102 (bigger joystick if you need to compensate for any inadequacy)
both joysticks are 10Kohm(ers)  
press and hold the button to operate

no guranteease if she don't work 
I provided a bit of a dead spot center of the joystick 
*/

int button=10;
int sw;
const int x_axis = 0;
const int y_axis = 1;
const int solenoid_x1 = 3;
const int solenoid_x2 = 5;
const int solenoid_y1 = 6;
const int solenoid_y2 = 9;

int x_hi=0;
int x_lo=0;
int y_hi=0;
int y_lo=0;
int a=0;

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
  digitalWrite(button,HIGH);
  pinMode(solenoid_x1,OUTPUT);
  pinMode(solenoid_x2,OUTPUT);
  pinMode(solenoid_y1,OUTPUT);
  pinMode(solenoid_y2,OUTPUT);
}

void loop() {
  sw = digitalRead(button);
  if(sw==LOW)
  {
    a++;
    if (a==1){while(1){
      x_hi= analogRead(x_axis);
      x_hi= map(x_hi,0,512,255,0);
      analogWrite(solenoid_x1, x_hi);

      x_lo= analogRead(x_axis);
      x_lo= map(x_lo,518,1023,255,0);
      analogWrite(solenoid_x2, x_lo);

      y_hi= analogRead(y_axis);
      y_hi= map(y_hi,0,512,255,0);
      analogWrite(solenoid_y1, y_hi);

      y_lo= analogRead(y_axis);
      y_lo= map(y_lo,518,1023,255,0);
      analogWrite(solenoid_y2, y_lo);

      Serial.println(x_hi),(x_lo);
      Serial.println(y_hi),(y_lo);
      Serial.println(a);
      sw=digitalRead(button);
      if(sw==LOW){break;}
    }
    }
    if (a==2)
    {
      x_hi=0;
      analogWrite(solenoid_x1, x_hi);

      x_lo=0;
      analogWrite(solenoid_x2, x_lo);

      y_hi=0;
      analogWrite(solenoid_y1, y_hi);

      y_lo=0;
      analogWrite(solenoid_y2, y_lo);
      a=0;

      Serial.println(x_hi),(x_lo);
      Serial.println(y_hi),(y_lo);
      Serial.println(a);
    }        
    }
  }
