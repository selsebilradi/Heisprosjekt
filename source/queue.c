#include <stdlib.h>

#include "queue.h"
#include "hardware.h"


//ubrukelig funksjon, kast seinare
void popQueue(ElevatorOrder* queue, int length){
	for (int i = 0; i < length-1; i++){
		queue[i].floor = queue[i+1].floor;
		queue[i].orderType = queue[i+1].orderType;
	}
	queue[length-1].floor = -1;
	queue[length-1].orderType=NONE;
}

void deleteOrdersOnFloor(ElevatorOrder * queue, int length, double floor){
	if (floor != (int)floor){
		return;
	}

	//lol hardkoding deluxe coming right up
	int numberOfOrders = 0;
	for (int i = 0; i < length; i++){
		if (queue[i].floor == floor){
			numberOfOrders++;
		}
	}	

	if (numberOfOrders == 0){
		return;
	}

	int* indexes = malloc(numberOfOrders*sizeof(int));


	int offset=0;

	for (int i = 0; i < length; i++){
		if (queue[i].floor == floor){
			indexes[offset] = i;
			offset++;
		}
	}

	for (int i = 0; i < offset; i++){
		for (int j = indexes[i]; j < length-1 ;j++){
			queue[j-i].floor = queue[j-i+1];
			queue[j-i].orderType = queue[j-i+1].orderType;
		}
	}

	for (int i = length-numberOfOrders; i < length; i++){
		queue[i].floor=-1;
		queue[i].orderType=NONE;
	}

	free(indexes);
};