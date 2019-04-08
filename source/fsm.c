#include "hardware.h"
#include "elev.h"
#include "fsm.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N_BUTTONS 3

int dir;

static int queue_matrix[N_FLOORS][N_BUTTONS] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
};

void fsm_add_order(){
	for (int floor = 0; floor < N_FLOORS; floor++) { 	//N_FLOORS-1 = 3
		for (int button = 0; button < N_BUTTONS; button++) {
			if (elev_get_button_signal(button, floor)) {
				queue_matrix[floor][button] = 1;
				elev_set_button_lamp(button,floor,1);
			}
		}
	}
}

int fsm_has_order(){
	for (int floor=0; floor< N_FLOORS; floor++){ 	//N_FLOORS-1 = 3
		for(int button=0; button<N_BUTTONS; button++ ){
			if (queue_matrix[floor][button]) {  //returns 1 of there is an order
				return 1;
			}
		}
	}
	return 0;
}


/*
  ** FSM fsm_execute_orders **
  direction dir = que_get_dir();
  if dir != stopp
    motor dir = dir;
    state = vi ruller;
  else:
    dir = stopp;
    que_slett_ordre()
    skru av lys i knappene
    state = stå stille// openDOOR

*/
void remove_order(){
	int currentFloor = hw_floor_sensor_read();
	for(int button=0; button<N_BUTTONS; button++ ){
		queue_matrix[currentFloor][button] = 0;
		elev_set_button_lamp(button,currentFloor,0);
	}
}



int should_stop_at_floor(int currentFloor){
	printf("\n\nshould stop %d %d %d\n\n", queue_matrix[currentFloor][0], queue_matrix[currentFloor][1], queue_matrix[currentFloor][2]);
	for(int button=0; button<N_BUTTONS; button++ ){
		if(queue_matrix[currentFloor][button]){
			if(button == 0 && prev_dir == 1){
				return 1;
			}
			else if(button == 1 && prev_dir == -1){
				return 1;
			}
			else if(button == 2){
				return 1;
			}
		}
	}
	return 0;
}

state_t state_OPENDOOR(){
	if(get_timeElapsed() == 0){
		time_begin_timer();
	}
	hw_motor_dir(DIRN_STOP);
	hw_door_opener();
	if(time_is_up()){
		time_end_timer();
		hw_door_closer();
		remove_order();
		return idle;
	}
  return door;
}


state_t state_IDLE(){
	if(fsm_has_order()){
		return move;
	}
	return idle;
}

state_t state_MOVE(){
	int currentFloor = hw_floor_sensor_read();
	state_t state = move;
	if(currentFloor != -1){
		for (int floor=0; floor< N_FLOORS; floor++){ 	//N_FLOORS-1 = 3
			for(int button=0; button<N_BUTTONS; button++ ){
				if (queue_matrix[floor][button]){
					if(floor==currentFloor && should_stop_at_floor(floor)) {
						state = door;
					}
					else if(has_orders_above() && prev_dir == 1){
						dir = 1;
						hw_motor_dir(dir);
					}
					else if(has_orders_below() && prev_dir == -1){
						dir = -1;
						hw_motor_dir(dir);
					}
					else if(has_orders_above() && prev_dir == -1){
						dir = -1;
						hw_motor_dir(dir);
					}
					else if(has_orders_below() && prev_dir == 1){
						dir = 1;
						hw_motor_dir(dir);
					}
					else {
						dir = ((floor-currentFloor)/(abs(floor-currentFloor)));
						hw_motor_dir(dir);
					}

				}
			}
		}
		hw_order_light_outside(currentFloor);
		if (should_stop_at_floor(currentFloor)){
			state = door;
		}
	}
	return state;
}

state_t state_INIT(){
	elev_init();
	if(!(elev_init())) {
		 printf("Could not initialize \n");
	}

	while(1){
		if(hw_floor_sensor_read()<0) { //If sensor doesnt read floor it returns -1
			hw_motor_dir(DIRN_UP);
		}
		if(hw_floor_sensor_read()>=0) {
			elev_set_floor_indicator(hw_floor_sensor_read());
			hw_motor_dir(DIRN_STOP);
			return idle;
		}
	}
}

int has_orders_above(){
	int currentFloor = hw_floor_sensor_read();
	for (int floor=currentFloor+1; floor < N_FLOORS; floor++){
		for(int button=0; button<N_BUTTONS; button++ ){
			if(queue_matrix[floor][button]){
				return 1;
			}
		}
	}
	return 0;
}

int has_orders_below(){
	int currentFloor = hw_floor_sensor_read();
	for (int floor=0; floor < currentFloor; floor++){
		for(int button=0; button<N_BUTTONS; button++ ){
			if(queue_matrix[floor][button]){
				return 1;
			}
		}
	}
	return 0;
}
