#include "elevatorStateMachine.h"
#include <stdio.h>
#include "hardware.h"
#include "channels.h"
#include "io.h"
#include <stdlib.h>

void destination_reached(double floor){
	hardware_command_door_open(1); //skal være åpen i 3 sekunder. timer(3)?
	hardware_command_floor_indicator_on(floor);
}

void elevator_safety_function(){
	if (hardware_read_stop_signal()){
		hardware_command_stop_light(1);
		hardware_command_movement(HARDWARE_MOVEMENT_STOP);
		clearQueue(g_queue);
		if (hardware_read_floor_sensor(0)|| hardware_read_floor_sensor(1)||hardware_read_floor_sensor(2)|| hardware_read_floor_sensor(3)|| hardware_read_floor_sensor(4)){ //en definert etasje. skrive på en annen måte?
			state = DOOR_OPEN; //???
			hardware_command_door_open(1);
		}
	}
}

void empty_queue(){
	if (checkQueue(g_queue)){
		state = STANDING_STILL;
	}
}

int checkQueue(elevatorOrder * queue){
	if ((queue[0].floor==-1)&&(queue[0].orderType==NONE)){
		return 0;
	}
	else{
	return 1;
	}
}


void clearQueue(elevatorOrder * queue, int length){
	for (int i = 0; i < length; i++){
		queue[i].floor=-1;
		queue[i].orderType=NONE;
	}
}