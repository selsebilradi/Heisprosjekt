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
int g_floor;

void floorReached(int floor){
	hardware_command_door_open(1); 
	hardware_command_floor_indicator_on(floor);
	hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
	hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
	hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
	deleteOrdersOnFloor(g_queue, g_queue_length, floor);

	timer(3);
	hardware_command_door_open(0);

}


void elevatorSafetyFunction(){
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


void emptyQueue(){
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

void elevatorInit(){
	
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



void timer(int milliseconds){
	int time=milliseconds;
    while(1){
        milliseconds--;
		sleep(1);
        if (hardware_read_obstruction_signal()==1|| hardware_read_stop_signal()==1){
			timer(time);
			break;
		}
		if (milliseconds==0){
			break;
		}

    }
}

int destinationIsReached(){
	if (hardware_read_floor_sensor(g_queue[0].floor)){
		return 1;
	}
	else {
		return 0;
	}
}

void lights(){
	for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            /* Light for internal orders*/
            if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
                hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
				addOrder(f, HARDWARE_ORDER_INSIDE);
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
	int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
	elevatorInit();
	while(1){
		int floor1=hardware_read_floor_sensor(0);
		int floor2=hardware_read_floor_sensor(1);
		int floor3=hardware_read_floor_sensor(2);
		int floor4=hardware_read_floor_sensor(3);

		lights();

	switch (g_state)
	{
	case STANDING_STILL:
		checkQueue(g_queue);
		if (g_queue[i-1]>g_queue[i]){
			g_state=MOVE_UP;
		}
		if (g_queue[i-1]<g_queue[i]){
			g_state=MOVE_DOWN;
		}
		if(hardware_read_stop_signal()){
			elevatorSafetyFunction();

		}
		break;

	case DOOR_OPEN:
		hardware_command_movement(HARDWARE_MOVEMENT_STOP);
		floorReached(g_floor);
		if(hardware_read_stop_signal()){
			elevatorSafetyFunction();

		}
		g_state=STANDING_STILL;
		break;

	case MOVE_UP:
		hardware_command_movement(HARDWARE_MOVEMENT_UP);
		if (destinationIsReached()){
			g_state=DOOR_OPEN;
		}
		if(hardware_read_stop_signal()){
			elevatorSafetyFunction();

		}
		break;

	case MOVE_DOWN:
		hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
		if (destinationIsReached()){
			g_state=DOOR_OPEN;
		}
		if(hardware_read_stop_signal()){
			elevatorSafetyFunction();

		}
		break;

	default:
		break;
	}
	}
}




