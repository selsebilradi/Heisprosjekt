#define _DEFAULT_SOURCE

#include "elevatorStateMachine.h"
#include <stdio.h>
#include "hardware.h"
#include <stdlib.h>
#include "queue.h"
#include <signal.h>
#include <time.h>

State g_state;
int g_queue_length=10;
ElevatorOrder g_queue[10];
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
	clearQueue(g_queue, g_queue_length);
	clearAllOrderLights();
	hardware_command_movement(HARDWARE_MOVEMENT_STOP);
	hardware_command_stop_light(1);
		while(hardware_read_stop_signal()){
			if (hardware_read_floor_sensor(0)|| hardware_read_floor_sensor(1)||hardware_read_floor_sensor(2)|| hardware_read_floor_sensor(3)){
				hardware_command_door_open(1);

				g_state=DOOR_OPEN;
			}
		}
		hardware_command_stop_light(0);
		timer(3);
		hardware_command_door_open(0);
		g_state=STANDING_STILL;
		break;
	case 0:
		if (g_state!=DOOR_OPEN){
			break;
		}
		hardware_command_door_open(1);
		while(hardware_read_obstruction_signal()==1){
			checkAndSetLights();
			checkAndAddOrders();
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
	if (queue[0].floor==-1){
		return 0;
	}
	else{
		return 1;
	}
}

void clearQueue(ElevatorOrder * queue, int length){
	for (int i = 0; i < length; i++){
		queue[i].floor=-1;
		queue[i].orderType=HARDWARE_ORDER_INSIDE;
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
	g_state=STANDING_STILL;
	clearQueue(g_queue, g_queue_length);
}

void checkAndAddOrders(){
	for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
            if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
				addOrder(f, HARDWARE_ORDER_INSIDE);
				sortQueue();
				}

            if(hardware_read_order(f, HARDWARE_ORDER_UP)){
				addOrder(f, HARDWARE_ORDER_UP);
				sortQueue();
            }

            if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
				addOrder(f, HARDWARE_ORDER_DOWN);
				sortQueue();
            }
        }
		

}


void timer(int seconds){
	time_t startTime   = time(0);
	time_t currentTime = time(0);

	while (currentTime-startTime < seconds){ //subtraksjonen her e grei fordi POSIX-standarden seie at time_t e i sekund, og linux e POSIX-compliant
		checkAndAddOrders();
		checkAndSetLights();
        if (hardware_read_obstruction_signal()==1|| hardware_read_stop_signal()==1){
			timer(seconds);
			break;
		}
		currentTime = time(0);
	}
}


int checkDestination(){
	if (hardware_read_floor_sensor(g_queue[0].floor)){
		return 1;
	}
	else {
		return 0;
	}
}

void checkAndSetLights(){
	for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
            hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
		}

        if(hardware_read_order(f, HARDWARE_ORDER_UP)){
            hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
        }

        if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
            hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
        }
    }
}

void updateCurrentFloor(){
	for (int i=0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
		if (hardware_read_floor_sensor(i)){
			hardware_command_floor_indicator_on(i);
			g_floor=i;
		}
	}
}

void clearAllOrderLights(){
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
}

void printQueue(){
		printf("\n{ ");

	for (int i = 0; i < g_queue_length; i++){
		printf("{ %d, ", g_queue[i].floor);
	
		if (g_queue[i].orderType == HARDWARE_ORDER_INSIDE){
			printf("INSIDE");
		}
		else if (g_queue[i].orderType == HARDWARE_ORDER_UP){
			printf("UP");
		}
		else if (g_queue[i].orderType == HARDWARE_ORDER_DOWN){
			printf("DOWN");
		}

		if (i != g_queue_length - 1){
		printf(" }, ");
		}
		else{
			printf(" }");
		}
	}
	printf(" }\n\n");
}

static void sigint_handler(int sig){
    (void)(sig);
    printf("\nTerminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

int FSM(){

	int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
	signal(SIGINT, sigint_handler);
	
	elevatorInit();

	int stopped = 0;

	time_t tid = time(0);
	time_t temp;
	while(1){
		tid = time(0);
		if (tid % 1 == 0){
			if(temp != tid){
				printQueue();
			}
			temp = tid;
		}

		checkAndSetLights();
		checkAndAddOrders();
		updateCurrentFloor();
		sortQueue();

	switch (g_state)
	{
	case STANDING_STILL:
		if (checkQueue(g_queue)==1){

			if (g_floor<g_queue[0].floor){
				g_state = MOVE_UP;
			}

			else if (g_queue[0].floor<g_floor){
				g_state = MOVE_DOWN;
			}

			else if ((g_queue[0].floor == g_floor) && (stopped == 0)){
				g_state  = DOOR_OPEN;
			}
			else if ((g_queue[0].floor == g_floor) && (stopped == -1)){
				g_state  = MOVE_UP;
			}
			else if ((g_queue[0].floor == g_floor) && (stopped == 1)){
				g_state  = MOVE_DOWN;
			}
			if(hardware_read_stop_signal()){
			elevatorSafetyFunction();
			}
	}
		break;
		
	case DOOR_OPEN:
		hardware_command_movement(HARDWARE_MOVEMENT_STOP);
		floorReached(g_floor);
		if(hardware_read_stop_signal()||hardware_read_obstruction_signal()){
			elevatorSafetyFunction();

		}
		g_state=STANDING_STILL;
		break;

	case MOVE_UP:
		stopped = 0;
		sortQueue();
		hardware_command_movement(HARDWARE_MOVEMENT_UP);

		if(checkDestination()==1){
			g_state=DOOR_OPEN;
		
		}

		if (g_floor > g_queue[0].floor){
			for (int i = 1; i < g_queue_length; i++){
				if (g_floor == g_queue[i].floor){
					g_state = DOOR_OPEN;
				}
			}
		}

		if(hardware_read_stop_signal()){
			elevatorSafetyFunction();
			stopped = 1;
		}
		break;

	case MOVE_DOWN:
		stopped = 0;
		sortQueue();
		hardware_command_movement(HARDWARE_MOVEMENT_DOWN);

		if (checkDestination()){
			g_state=DOOR_OPEN;
	
		}

		if (g_floor < g_queue[0].floor){
			for (int i = 1; i < g_queue_length; i++){
				if (g_floor == g_queue[i].floor){
					g_state = DOOR_OPEN;
				}
			}
		}

		if(hardware_read_stop_signal()){
			elevatorSafetyFunction();
			stopped = -1;

		}

		break;

	default:
		break;
	}
	}
}




