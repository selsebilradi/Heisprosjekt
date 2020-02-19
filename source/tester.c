#include <stdio.h>
#include "hardware.h"
#include <unistd.h>
#include <stdlib.h>
#include "elevatorStateMachine.h"

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

void lights(){
	for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            /* Light for internal orders*/
            if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
                hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
				addOrder(f, HARDWARE_ORDER_INSIDE);
				for (int j=0; j<g_queue_length;j++){
					printf("%d\n", g_queue[j].floor);
				}
            }

            /* Lights for orders going up*/
            if(hardware_read_order(f, HARDWARE_ORDER_UP)){
                hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
				addOrder(f, HARDWARE_ORDER_UP);
            }

            /* Lights for orders going down */
            if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
                hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
				addOrder(f, HARDWARE_ORDER_DOWN);
            }
        }
		
		//Indicates which floor the elevator is on
		for (int i=0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
			if (hardware_read_floor_sensor(i)){
				hardware_command_floor_indicator_on(i);
				g_floor=i;
			}

		}
}

int main(){
	lights();
	return 0;
}