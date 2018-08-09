#include "valve_controller.h" 

/************************************************************/
/**
 * Set the valve's position
 * 
 * @param valve The valve 
 * @param value The value for that valve positive if its going applying pressure to port A or 
 *              negative if its applying pressure to port B
 **/
void set_valve_pos(Valve *valve, int value) 
{
	if (value > 0)
		analogWrite(valve->valve_pins.pos, value);
	else
		analogWrite(valve->valve_pins.neg, value);
}
/************************************************************/
/**
 * Set the ramp position
 * 
 * @param ramp The ramp time in ms
 **/
void set_ramp(int value) 
{
	ramp_value = value; 
}
/************************************************************/

