#include "elevatorStateMachine.h"
#include <stdio.h>
#include "hardware.h"
#include <stdlib.h>
#include "queue.h"
#include <unistd.h>
#include <stdlib.h>

State g_state;
int g_queue_length;
ElevatorOrder g_queue[6];
double g_floor;

void destination_reached(double floor){
	hardware_command_door_open(1); 
	hardware_command_floor_indicator_on(floor);
	hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
	hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
	hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
	deleteOrdersOnFloor(g_queue, g_queue_length, floor);

	timer(3);
	hardware_command_door_open(0);

}


void elevator_safety_function(){
	switch (hardware_read_stop_signal())
	{
	case 1:
	clearQueue(g_queue);
	hardware_command_movement(HARDWARE_MOVEMENT_STOP);
	hardware_command_stop_light(1);
		while(hardware_read_stop_signal()){
			if (hardware_read_floor_sensor(0)|| hardware_read_floor_sensor(1)||hardware_read_floor_sensor(2)|| hardware_read_floor_sensor(3)){
				hardware_command_door_open(1);
				g_state=DOOR_OPEN;
			}
		}
		hardware_command_stop_light(0);
		g_state=STANDING_STILL;
		timer(3);
		hardware_command_door_open(0);
		break;
	case 0:
		if (g_state!=DOOR_OPEN){
			break;
		}
		hardware_command_door_open(1);
		while(hardware_read_obstruction_signal()==1){
		}
		
		timer(3);
		hardware_command_door_open(0);
		break;
	default:
		break;
	}
}


void empty_queue(){
	if (checkQueue(g_queue)==0){
		g_state = STANDING_STILL;
		hardware_command_door_open(0);
	}
}



int checkQueue(ElevatorOrder * queue){
	if ((queue[0].floor==-1)&&(queue[0].orderType==NONE)){
		return 0;
	}
	else{
		return 1;
	}
}


void clearQueue(ElevatorOrder * queue, int length){
	for (int i = 0; i < length; i++){
		queue[i].floor=-1;
		queue[i].orderType=NONE;
	}
}

void elevator_init(){
	
	g_floor=0;
	hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
	while (1){
		if (hardware_read_floor_sensor(g_floor)==1){
			hardware_command_movement(HARDWARE_MOVEMENT_STOP);
			break;
		}
		
	}
	HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }

	hardware_command_floor_indicator_on(g_floor);
	State=STANDING_STILL;
	clearQueue(g_queue);
	}



// denne er ikke ferdig.
void timer(int milliseconds){
    while(1){
        milliseconds--;
		sleep(0.001);
        if (hardware_read_obstruction_signal()==1|| hardware_read_stop_signal()==1){
			break;
		}

    }
}
