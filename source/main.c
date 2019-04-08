#include "elev.h"
#include <stdio.h>
#include "hardware.h"
#include "fsm.h"






int main(){

      // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    //fsm_go_to_idle(); //if between two floors, go to nearest upper floor
    state_t state = state_INIT();
    while (1) {


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

         case init:
         state = state_INIT();
         break;
 }



           //BARE ET EKSEMPEL
            //if has_orders {
              //  dir = calc_dir()
               // state = moving
            //}
        //}


            //elev_set_motor_direction(dir);

        //fsm_check_orders();
        //if(has_orders()){
         //   printQueue();
        //}





       // fsm_execute_orders(queue_matrix);




        //hw_check_orders();



      /* if(elev_get_floor_sensor_signal() == 3){ //fourth floor
            elev_set_motor_direction(DIRN_STOP);
            elev_set_door_open_lamp(1);
        }


        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }*/











        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }


    return 0;
}
