if (hardware_read_stop_signal()==1){
		hardware_command_stop_light(1);
		hardware_command_movement(HARDWARE_MOVEMENT_STOP);
		clearQueue(g_queue);
		if (hardware_read_floor_sensor(0)|| hardware_read_floor_sensor(1)||hardware_read_floor_sensor(2)|| hardware_read_floor_sensor(3)){ //en definert etasje. skrive på en annen måte?
			g_state = DOOR_OPEN; //???
			hardware_command_door_open(1);
		}
	}



void addOrder(int floor,HardwareOrder orderType){
	int duplicate=0;
	for (int i=0; i<g_queue_length; i++){
		if (g_queue[i].floor==floor && g_queue[i].orderType== orderType){
			duplicate=1;	
		}	
		
	}
	
	if (duplicate==0){
		for (int j=0; j<g_queue_length; j++){
				if(g_queue[j].floor==-1 && g_queue[j].orderType==NONE){
					g_queue[j].floor=floor;
					g_queue[j].orderType=orderType;
					break;
	}
	
}



//ubrukelig funksjon, kast seinare
void popQueue(ElevatorOrder* queue, int length){
	for (int i = 0; i < length-1; i++){
		queue[i].floor = queue[i+1].floor;
		queue[i].orderType = queue[i+1].orderType;
	}
	queue[length-1].floor = -1;
	queue[length-1].orderType=HARDWARE_ORDER_INSIDE;
}



// denne er ikke ferdig.
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
    

if(hardware_read_order(1,HARDWARE_ORDER_UP)){
	hardware_command_order_light(1,HARDWARE_ORDER_UP,1);
	hardware_command_movement(HARDWARE_MOVEMENT_STOP);

	    timer(3);		
	hardware_command_movement(HARDWARE_MOVEMENT_UP);
	hardware_command_order_light(1,HARDWARE_ORDER_UP,0);
	}


static void sigint_handler(int sig){
	(void)(sig);
	printf("Terminating elevator\n");
	exit(1);
}



void printQueue(ElevatorOrder* queue, int length){
		printf("\n{ ");

	for (int i = 0; i < length; i++){
		printf("{ %d, ", queue[i].floor);
		if (queue[i].orderType == NONE){
			printf("NONE");
		}
		else if (queue[i].orderType == HARDWARE_ORDER_INSIDE){
			printf("INSIDE");
		}
		else if (queue[i].orderType == HARDWARE_ORDER_UP){
			printf("UP");
		}
		else if (queue[i].orderType == HARDWARE_ORDER_DOWN){
			printf("DOWN");
		}

		if (i != length - 1){
		printf(" }, ");
		}
		else{
			printf(" }");
		}
	}
	printf(" }\n\n");
}

void popQueue(ElevatorOrder* queue, int length){
	for (int i = 0; i < length-1; i++){
		queue[i].floor = queue[i+1].floor;
		queue[i].orderType = queue[i+1].orderType;
	}
	queue[length-1].floor = -1;
	queue[length-1].orderType=HARDWARE_ORDER_INSIDE;
}



void timer(int mcseconds){
	int time=mcseconds;
    while(1){
        mcseconds--;
		checkAndAddOrders();
		checkAndSetLights();
		usleep(1);
        if (hardware_read_obstruction_signal()==1|| hardware_read_stop_signal()==1){
			timer(time);
			break;
		}
		if (mcseconds==0){
			break;
		}

    }
}