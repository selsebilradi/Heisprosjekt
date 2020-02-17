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
    