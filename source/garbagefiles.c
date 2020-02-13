if (hardware_read_stop_signal()==1){
		hardware_command_stop_light(1);
		hardware_command_movement(HARDWARE_MOVEMENT_STOP);
		clearQueue(g_queue);
		if (hardware_read_floor_sensor(0)|| hardware_read_floor_sensor(1)||hardware_read_floor_sensor(2)|| hardware_read_floor_sensor(3)){ //en definert etasje. skrive på en annen måte?
			g_state = DOOR_OPEN; //???
			hardware_command_door_open(1);
		}
	}

    