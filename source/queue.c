#include <stdlib.h>

#include "queue.h"
#include "hardware.h"
#include "elevatorStateMachine.h"
//extern ElevatorOrder g_queue[6];

//ubrukelig funksjon, kast seinare
void popQueue(ElevatorOrder* queue, int length){
	for (int i = 0; i < length-1; i++){
		queue[i].floor = queue[i+1].floor;
		queue[i].orderType = queue[i+1].orderType;
	}
	queue[length-1].floor = -1;
	queue[length-1].orderType=HARDWARE_ORDER_INSIDE;
}

void deleteOrdersOnFloor(ElevatorOrder * queue, int length, int floor){
	
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
			queue[j-i].floor = queue[j-i+1].floor;
			queue[j-i].orderType = queue[j-i+1].orderType;
		}
	}

	for (int i = length-numberOfOrders; i < length; i++){
		queue[i].floor=-1;
		queue[i].orderType=HARDWARE_ORDER_INSIDE;
	}

	free(indexes);
};

void addOrder(int floor,HardwareOrder orderType){
	int duplicate=0;
	for (int i=0; i<g_queue_length; i++){
		if (g_queue[i].floor==floor && g_queue[i].orderType== orderType){
			duplicate=1;	
		}	
		
	}
	
	if (duplicate==0){
		for (int j=0; j<g_queue_length; j++){
				if(g_queue[j].floor==-1 && g_queue[j].orderType==HARDWARE_ORDER_INSIDE){
					g_queue[j].floor=floor;
					g_queue[j].orderType=orderType;
					break;
		}
	
	}
}
}