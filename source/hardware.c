#include "elev.h"
#include "io.h"



void hw_door_opener() {
	elev_set_door_open_lamp(1);
}

void hw_door_closer() {
	elev_set_door_open_lamp(0);
}


int hw_floor_sensor_read() {
	//int flr = (elev_get_floor_sensor_signal());
	return (elev_get_floor_sensor_signal());
}

void hw_order_light_outside(int flr) {
	elev_set_floor_indicator(flr);
}

void hw_turn_on_button_light(int flr, int dir) {
	if (dir > 0) {
		elev_set_button_lamp(BUTTON_CALL_UP, flr, 1);
	}
	else if (dir == 0) {
		elev_set_button_lamp(BUTTON_COMMAND, flr, 1);
	}
	else if (dir < 0) {
		elev_set_button_lamp(BUTTON_CALL_DOWN, flr, 1);
	}
}

void hw_turn_off_button_light(int flr, int dir) {
	if (dir > 0) {
		elev_set_button_lamp(BUTTON_CALL_UP, flr, 1);
	}
	else if (dir == 0) {
		elev_set_button_lamp(BUTTON_COMMAND, flr, 1);
	}
	else if (dir < 0) {
		elev_set_button_lamp(BUTTON_CALL_DOWN, flr, 1);
	}
}


void hw_button_emerg_stop() {
	if (elev_get_stop_signal()) {
		elev_set_stop_lamp(1);
		//motor_pwr(0);
		if (hw_floor_sensor_read()) {
			elev_set_door_open_lamp(1);
			//sleep(3);
			elev_set_door_open_lamp(0);
		}
		//Slett alle bestillinger, men heisen skal vite hvor den er
	}
}

/**
Gets a button signal.
@param button Which button type to check. Can be BUTTON_CALL_UP,
BUTTON_CALL_DOWN or BUTTON_COMMAND (button "inside the elevator).
@param floor Which floor to check button. Must be 0-3.
@return 0 if button is not pushed. 1 if button is pushed.
*/
