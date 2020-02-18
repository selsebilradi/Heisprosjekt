#define _DEFAULT_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include <unistd.h>

void timer(int milliseconds){
	int time=milliseconds;
    while(1){
        milliseconds--;
		usleep(1);
        if (hardware_read_obstruction_signal()==1|| hardware_read_stop_signal()==1){
            hardware_command_floor_indicator_on(2);
			timer(time);
			break;
		}
		if (milliseconds==0){
            hardware_command_floor_indicator_on(1);
			break;
		}

    }
}

int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    hardware_command_movement(HARDWARE_MOVEMENT_UP);

    while(1){
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            break;
        }

        if(hardware_read_floor_sensor(0)){
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        if(hardware_read_floor_sensor(HARDWARE_NUMBER_OF_FLOORS - 1)){
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
	if(hardware_read_order(1,HARDWARE_ORDER_UP)){
	   hardware_command_order_light(1, HARDWARE_ORDER_UP,1);
	   hardware_command_movement(HARDWARE_MOVEMENT_STOP);
	   timer(30000);	
	   hardware_command_order_light(1, HARDWARE_ORDER_UP,0);
	   hardware_command_movement(HARDWARE_MOVEMENT_UP);
	
	}
    }
}
