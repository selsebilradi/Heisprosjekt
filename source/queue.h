#ifndef QUEUE_H
#define QUEUE_H

#include "hardware.h"

typedef struct{
	double floor; //lagrar som double for seinare bruk med sjekk om den er mellom etasjer. 
	HardwareOrder orderType;
}ElevatorOrder;

 
void popQueue(ElevatorOrder* queue);

void sortQueue(ElevatorOrder* queue);


#endif