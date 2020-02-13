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
	hardware_command_door_open(1); //skal være åpen i 3 sekunder. timer(3)?
	hardware_command_floor_indicator_on(floor);
	hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
	hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
	hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
	popQueue(g_queue);

}

void elevator_safety_function(){
	if (hardware_read_stop_signal()){
		hardware_command_stop_light(1);
		hardware_command_movement(HARDWARE_MOVEMENT_STOP);
		clearQueue(g_queue);
		if (hardware_read_floor_sensor(0)|| hardware_read_floor_sensor(1)||hardware_read_floor_sensor(2)|| hardware_read_floor_sensor(3)){ //en definert etasje. skrive på en annen måte?
			g_state = DOOR_OPEN; //???
			hardware_command_door_open(1);
		}
	}
}

void empty_queue(){
	if (checkQueue(g_queue)){
		g_state = STANDING_STILL;
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
	hardware_command_floor_indicator_on(g_floor);
	State=STANDING_STILL;
	clearQueue(g_queue);
	}
	
void timer(int seconds){
    while(1){
        seconds--;
		sleep(1);
        if (hardware_read_obstruction_signal()==1|| hardware_read_stop_signal()==1){
			break;
		}

    }
}
