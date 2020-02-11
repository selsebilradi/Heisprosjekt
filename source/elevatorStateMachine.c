#include "elevatorStateMachine.h"
#include <stdio.h>
#include "hardware.h"
#include "channels.h"
#include "io.h"
#include <stdlib.h>

void destination_reached(double floor){
	hardware_command_door_open(1);
	hardware_command_floor_indicator_on(floor);
}