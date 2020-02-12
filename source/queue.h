#ifndef QUEUE_H
#define QUEUE_H

#include "hardware.h"

typedef struct{
	double floor; //lagrar som double for seinare bruk med sjekk om den er mellom etasjer. 
	HardwareOrder orderType;
}elevatorOrder;

 
void popQueue(elevatorOrder* queue);

void sortQueue(elevatorOrder* queue);


#endif