#include <stdio.h>
#include "hardware.h"
#include <stdlib.h>
#include "queue.h"
#include <unistd.h>
#include <stdlib.h>

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