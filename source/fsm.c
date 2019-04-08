#include "hardware.h"
#include "elev.h"
#include "fsm.h"
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


void fsm_go_to_idle() {
	elev_init();
	if(!(elev_init())) {
		 //printf('Could not initialize');
	}

	while(1){
		if(hw_floor_sensor_read()<0) { //If sensor doesnt read floor it returns -1
			elev_set_motor_direction(DIRN_UP);
		}
		if(hw_floor_sensor_read()>=0) {
			elev_set_motor_direction(DIRN_STOP);
      hw_order_light_outside(hw_floor_sensor_read());
			break;
		}
	}
}


void fsm_add_order(){
  for (int floor = 0; floor < N_FLOORS; floor++) { 	//N_FLOORS-1 = 3
  	for (int button = 0; button < N_BUTTONS; button++) {
  		if (elev_get_button_signal(button, floor)) {
  			if (button % 2 == 0 && button == 0) {
  				queue_matrix[floor][button] = 1;
  			}
  			if (button % 2 == 1) {
  				queue_matrix[floor][button] = -1;
  			}
  			if (button % 2 == 0 && button == 2) {
  				queue_matrix[floor][button] = 2;
        }
	   			elev_set_button_lamp(button,floor,1);
            }
       }
    }
}

int fsm_has_order(){
	for (int floor=0; floor< N_FLOORS; floor++){ 	//N_FLOORS-1 = 3
	   for(int button=0; button<N_BUTTONS; button++ ){
	   		if (queue_matrix[floor][button] != 0) {  //returns 1 of there is an order
	   			return 1;
            }
	   	}
    }
    return 0;
  }

state_t fsm_execute_orders(){
  state_t state = idle;
	int currentFloor = hw_floor_sensor_read();
  for (int floor=0; floor< N_FLOORS; floor++){ 	//N_FLOORS-1 = 3
	   for(int button=0; button<N_BUTTONS; button++ ){
       if (queue_matrix[floor][button] != 0){
         dir = ((floor-currentFloor)/(abs(floor-currentFloor)));
       }
	   			elev_set_motor_direction(dir); //drive motorb
	   		}
      }
      if(hw_floor_sensor_read() != -1){
       hw_order_light_outside(currentFloor);
        state = check_order_at_floor(currentFloor);
	}
  return state;
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




state_t check_order_at_floor(int currentFloor){
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
  fsm_add_order();
  //sjekke knapper
  // etg lys settes (4 safety)
  //åpne dør
  //etter tre sek return fsm_has_orders
  sleep(3);
  return idle;
}


state_t state_IDLE(){
  fsm_add_order();
  // sjekk knapper
  if(fsm_has_order()){
    return move;
  }
  return idle;
}

state_t state_MOVE(){
  fsm_add_order();
  //fsm_execute_orders();
  //sjekke kanpper
  // etg. lys settes
  return fsm_execute_orders();
}

state_t state_INIT(){
	elev_init();
	if(!(elev_init())) {
		 //printf('Could not initialize');
	}

	while(1){
		if(hw_floor_sensor_read()<0) { //If sensor doesnt read floor it returns -1
			elev_set_motor_direction(DIRN_UP);
		}
		if(hw_floor_sensor_read()>=0) {
			elev_set_motor_direction(DIRN_STOP);
			return idle;
		}
	}
}
