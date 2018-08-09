#ifndef __VALVE_CONTROLLER_H
#define __VALVE_CONTROLLER_H

typedef struct valve_pins { 
    int pos; //Positive pin 
    int neg; //Negative pin
} ValvePins; 

typedef struct valve { 
    ValvePins valve_pins; 

} Valve; 

Valve *valves; 

int ramp_value; 

void set_valve_pos(Valve*, int);
void set_ramp(int);

#endif