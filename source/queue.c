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

void deleteOrdersOnFloor(ElevatorOrder * queue, int length, double floor){};