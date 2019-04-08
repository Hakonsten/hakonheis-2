#pragma once

//Door
void hw_door_opener();
void hw_door_closer();



//Sensor and lights
int hw_floor_sensor_read();
void hw_order_light_outside(int flr);
void hw_turn_on_button_light(int flr, int dir);
void hw_turn_off_button_light(int flr, int dir);



//button functions
void hw_button_emerg_stop();
