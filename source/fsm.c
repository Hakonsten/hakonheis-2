#include "hardware.h"
#include "elev.h"
#include "fsm.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N_BUTTONS 3

int dir= 0;

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



state_t get_order_at_floor(int currentFloor){
  for(int button=0; button<N_BUTTONS; button++ ){
    if(queue_matrix[currentFloor][button] != 0 ){
      if((dir == (queue_matrix[currentFloor][button])) || ((queue_matrix[currentFloor][button]) == 2)){
        queue_matrix[currentFloor][button] = 0;
        elev_set_button_lamp(button,currentFloor,0);
        return door;
      }
    }
    return idle;
  }
  return idle;
}

state_t state_OPENDOOR(){
	if(get_timeElapsed() == 0){
		time_begin_timer();
	}
	elev_set_motor_direction(DIRN_STOP);
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
					if(floor==currentFloor) {
						state = door;
					}
					else{
						dir = ((floor-currentFloor)/(abs(floor-currentFloor)));
						elev_set_motor_direction(dir);
					}
				}
			}
		}
		hw_order_light_outside(currentFloor);
		if (get_order_at_floor(currentFloor)){
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
			elev_set_motor_direction(DIRN_UP);
		}
		if(hw_floor_sensor_read()>=0) {
			elev_set_floor_indicator(hw_floor_sensor_read());
			elev_set_motor_direction(DIRN_STOP);
			return idle;
		}
	}
}
