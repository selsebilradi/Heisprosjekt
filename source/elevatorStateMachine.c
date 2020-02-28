
#include "elevatorStateMachine.h"
#include <stdio.h>
#include "hardware.h"
#include <stdlib.h>
#include "queue.h"
#include <signal.h>
#include <time.h>

State g_state;
State g_prev_state;
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
	clearQueue();
	clearAllOrderLights();
	hardware_command_movement(HARDWARE_MOVEMENT_STOP);
	hardware_command_stop_light(1);

		while(hardware_read_stop_signal()){
			if (hardware_read_floor_sensor(0) || hardware_read_floor_sensor(1) || hardware_read_floor_sensor(2) || hardware_read_floor_sensor(3)){
				hardware_command_door_open(1);

				g_state = DOOR_OPEN;
			}
		}

		hardware_command_stop_light(0);
		timer(3);
		hardware_command_door_open(0);
		g_state = STANDING_STILL;
		break;
	case 0:
		if (g_state != DOOR_OPEN){
			break;
		}

		hardware_command_door_open(1);

		while(hardware_read_obstruction_signal() == 1){
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

void elevatorInit(){
	
	g_floor = 0;
	hardware_command_movement(HARDWARE_MOVEMENT_DOWN);

	while (1){
		if (hardware_read_floor_sensor(g_floor) == 1){
			hardware_command_movement(HARDWARE_MOVEMENT_STOP);
			break;
		}
	}

	HardwareOrder orderTypes[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = orderTypes[i];
            hardware_command_order_light(f, type, 0);
        }
    }

	hardware_command_floor_indicator_on(g_floor);
	g_state = STANDING_STILL;
	g_prev_state = STANDING_STILL;
	clearQueue();
}

void checkAndAddOrders(){
	for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
			addOrder(f, HARDWARE_ORDER_INSIDE);
		}

        if(hardware_read_order(f, HARDWARE_ORDER_UP)){	
			addOrder(f, HARDWARE_ORDER_UP);
        }

        if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
			addOrder(f, HARDWARE_ORDER_DOWN);
        }
    }
}


void timer(int seconds){
	time_t startTime   = time(0);
	time_t currentTime = time(0);

	while (currentTime-startTime < seconds){ //subtraksjonen her e grei fordi POSIX-standarden seie at time_t e i sekund, og linux e POSIX-compliant
		checkAndSetLights();
		checkAndAddOrders();
        if (hardware_read_obstruction_signal() == 1 || hardware_read_stop_signal() == 1){
			elevatorSafetyFunction();
			break;
		}

		if((hardware_read_order(g_floor, HARDWARE_ORDER_UP) || hardware_read_order(g_floor, HARDWARE_ORDER_DOWN) || hardware_read_order(g_floor, HARDWARE_ORDER_INSIDE)) && (g_state == DOOR_OPEN)){
			timer(3);
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

		if((f == g_floor) && (g_state == DOOR_OPEN)){
			hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 0);
			hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 0);
			hardware_command_order_light(f, HARDWARE_ORDER_UP, 0);
		}
    }
}

void updateCurrentFloor(){
	for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
		if (hardware_read_floor_sensor(i)){
			hardware_command_floor_indicator_on(i);
			g_floor = i;
		}
	}
}

void clearAllOrderLights(){
    HardwareOrder orderTypes[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = orderTypes[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}


static void sigint_handler(int sig){
    (void)(sig);
    printf("\nTerminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

int FSM(){
	printf("Initializing elevator... Please wait.\n");
	int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

	signal(SIGINT, sigint_handler);
	
	elevatorInit();
	printf("The elevator is now initialized.\nYou may order at your leisure.\n");

	int stopped = 0;

	while(1){
		
		checkAndSetLights();
		checkAndAddOrders();
		updateCurrentFloor();
		sortQueue();

		if (hardware_read_floor_sensor(0) || hardware_read_floor_sensor(1) || hardware_read_floor_sensor(2) || hardware_read_floor_sensor(3)){
			stopped = 0;
		}

	switch (g_state)
	{
		case STANDING_STILL:
			if(hardware_read_stop_signal()){
				elevatorSafetyFunction();
				g_prev_state = STANDING_STILL;
			}

			if (checkQueue() == 1){

				if (g_floor < g_queue[0].floor){
					g_state = MOVE_UP;
					g_prev_state = STANDING_STILL;
				}

				else if (g_queue[0].floor < g_floor){
					g_state = MOVE_DOWN;
					g_prev_state = STANDING_STILL;
				}

				else if ((g_queue[0].floor == g_floor) && (stopped == 0)){
					g_state  = DOOR_OPEN;
					g_prev_state = STANDING_STILL;
				}

				else if ((g_queue[0].floor == g_floor) && (stopped == -1)){
					g_state  = MOVE_UP;
					g_prev_state = STANDING_STILL;
				}

				else if ((g_queue[0].floor == g_floor) && (stopped == 1)){
					g_state  = MOVE_DOWN;
					g_prev_state = STANDING_STILL;	
				}	
			}
			break;
			
		case DOOR_OPEN:
			hardware_command_movement(HARDWARE_MOVEMENT_STOP);
			floorReached(g_floor);

			if(hardware_read_stop_signal()||hardware_read_obstruction_signal()){
				elevatorSafetyFunction();
				g_prev_state = DOOR_OPEN;
			}

			g_state = STANDING_STILL;
			g_prev_state = DOOR_OPEN;
			break;

		case MOVE_UP:
			sortQueue();
			hardware_command_movement(HARDWARE_MOVEMENT_UP);

			if(checkDestination() == 1){
				g_state = DOOR_OPEN;
				g_prev_state = MOVE_UP;
			}

			if(hardware_read_stop_signal()){
				elevatorSafetyFunction();
				g_prev_state = MOVE_UP;

				if (stopped == 0){
					stopped = 1;
				}
			}

			break;

		case MOVE_DOWN:
			sortQueue();
			hardware_command_movement(HARDWARE_MOVEMENT_DOWN);

			if (checkDestination()){
				g_state = DOOR_OPEN;
				g_prev_state = MOVE_DOWN;
			}

			if(hardware_read_stop_signal()){
				elevatorSafetyFunction();
				g_prev_state = MOVE_DOWN;
				if (stopped == 0){
					stopped = -1;
				}
			}

			break;

		default:
			break;
		}
	}
}




