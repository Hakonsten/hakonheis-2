#include "elev.h"
#include <stdio.h>
#include "hardware.h"
#include "fsm.h"

int main(){

    //fsm_go_to_idle(); //if between two floors, go to nearest upper floor
    state_t state = state_INIT();
    while (1) {
		printf("state: %d\n", state);
		fsm_add_order();

		switch (state) {
			case idle:
			state = state_IDLE();
			break;

			case move:
			state = state_MOVE();
			break;

			case door:
			state = state_OPENDOOR();
	 		break;
	 	}

		if (elev_get_stop_signal()) {
			elev_set_motor_direction(DIRN_STOP);
			break;
		}
	}


    return 0;
}
