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
		break;
	}

	int* indexes = malloc(numberOfOrders*sizeof(int));


	int offset=0;

	for (int i = 0; i < length; i++){
		if (queue[i].floor == floor){
			indexes[offset] = i;
			offset++;
		}
	}

	for (int i = 0; i <= offset; i++;){
		for (int j = indexes[i]-1; j < length-1 ;j++){
			queue[j].floor = queue[j+1];
			queue[j].orderType = queue[j+1].orderType;
		}
	}

	for (int i = length-offset-1; i < length; i++){
		queue[i].floor=-1;
		queue[i].orderType=NONE;
	}

	free(indexes);
};