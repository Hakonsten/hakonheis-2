#pragma once
//Important global variables that should be used through out
int currentFloor;
int currentState;



void fsm_go_to_idle(); //go to idle
void fsm_add_order();
int fsm_has_order();
void fsm_print_queue();



//typedef enum states {state_INIT,state_IDLE,state_OPENDOOR,state_MOVE,state_EMERGENCY_STOP} state_t;
typedef enum states {idle, move, door} state_t;

state_t state_start();
state_t state_IDLE();
state_t state_OPENDOOR();
state_t state_INIT();
state_t state_MOVE();
state_t state_EMERGENCY_STOP();
state_t get_order_at_floor(int currentFloor);
state_t fsm_execute_orders();
