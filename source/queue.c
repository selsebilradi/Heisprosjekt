#include <stdlib.h>

#include "queue.h"
#include "hardware.h"
#include "elevatorStateMachine.h"

void deleteOrdersOnFloor(ElevatorOrder * queue, int length, int floor){
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
			if(g_queue[j].floor==-1 && g_queue[j].orderType==HARDWARE_ORDER_INSIDE){
				g_queue[j].floor=floor;
				g_queue[j].orderType=orderType;
				break;
			}
	
		}
	}
}

void sortQueue(){
    int OrdersUp=0;
    int OrdersDown=0;
    int OrdersInside=0;

    for (int i=0; i<g_queue_length; i++){
        if (g_queue[i].orderType==HARDWARE_ORDER_INSIDE && g_queue[i].floor!=-1){
            OrdersInside++;
        }
        else if(g_queue[i].orderType==HARDWARE_ORDER_UP){
            OrdersUp++;

        }
        else if(g_queue[i].orderType==HARDWARE_ORDER_DOWN){
            OrdersDown++;
        }
    }
    switch (g_state)
    {
    case MOVE_UP:   ;
    {//Lagar eige scope her, slik at me slepp trøbbel med redefinition av variable
        int numberOfRelevantOrders = OrdersUp + OrdersInside;
        ElevatorOrder * relevantOrders = (ElevatorOrder *) malloc(numberOfRelevantOrders * sizeof(ElevatorOrder));
        ElevatorOrder * downOrders = (ElevatorOrder *) malloc((OrdersDown)*sizeof(ElevatorOrder));
        int relIndex = 0;
        int downIndex = 0;

        for (int j = 0; j < g_queue_length; j++){
            if (g_queue[j].orderType != HARDWARE_ORDER_DOWN && g_queue[j].floor!=-1){
                relevantOrders[relIndex].floor     = g_queue[j].floor;
                relevantOrders[relIndex].orderType = g_queue[j].orderType;
                relIndex++;
            }
            else if(g_queue[j].orderType == HARDWARE_ORDER_DOWN){
                downOrders[downIndex].floor = g_queue[j].floor;
                downOrders[downIndex].orderType = g_queue[j].orderType;
                downIndex++;
            }
        }
        //bubblesort
        int swapped, count;
        ElevatorOrder temp;
        while(1){
            if (OrdersUp+OrdersInside==0){
                break;
            }
            swapped=0;
            for (count=0; count<OrdersUp+OrdersInside-1;count++){
                if (relevantOrders[count].floor > relevantOrders[count+1].floor){
                    temp.floor=relevantOrders[count].floor;
                    temp.orderType=relevantOrders[count].orderType;

                    relevantOrders[count].floor=relevantOrders[count+1].floor;
                    relevantOrders[count].orderType=relevantOrders[count+1].orderType;

                    relevantOrders[count+1].floor=temp.floor;
                    relevantOrders[count+1].orderType=temp.orderType;

                    swapped=1;
                }
            }
            if (swapped==0){
                break;
            }
        }

        
        int queueIndex=0;
        int fromIndex=0;
        for (int i =0; i<OrdersUp+OrdersInside; i++){
            if (relevantOrders[i].floor>=g_floor){
                fromIndex=i;
                break;
            }
        }
        for (int i =fromIndex; i<OrdersUp+OrdersInside; i++){
            g_queue[queueIndex].floor=relevantOrders[i].floor;
            g_queue[queueIndex].orderType=relevantOrders[i].orderType;
            queueIndex++;
        }
        
        for(int i=0; i<fromIndex; i++){
            g_queue[queueIndex].floor=relevantOrders[i].floor;
            g_queue[queueIndex].orderType=relevantOrders[i].orderType;
            queueIndex++;

        }
        for(int i=0; i<downIndex; i++){
            g_queue[queueIndex].floor=downOrders[i].floor;
            g_queue[queueIndex].orderType=downOrders[i].orderType;
            queueIndex++;
        }

        for (int i = 0; i < g_queue_length-OrdersInside-OrdersUp-OrdersDown; i++){
            g_queue[queueIndex].floor     = -1;
            g_queue[queueIndex].orderType = HARDWARE_ORDER_INSIDE;
            queueIndex++;
        }


        free(relevantOrders);
        free(downOrders);
    }
        break;
    case MOVE_DOWN:
    {//Lagar eige scope her, slik at me slepp trøbbel med redefinition av variable
        int numberOfRelevantOrders = OrdersDown + OrdersInside;
        ElevatorOrder* relevantOrders=(ElevatorOrder*)malloc(numberOfRelevantOrders * sizeof(ElevatorOrder));
        ElevatorOrder* upOrders=(ElevatorOrder*)malloc((OrdersUp)*sizeof(ElevatorOrder));

        int relIndex = 0;
        int upIndex = 0;

        for (int j = 0; j < g_queue_length; j++){
            if (g_queue[j].orderType != HARDWARE_ORDER_UP && g_queue[j].floor!=-1){
                relevantOrders[relIndex].floor     = g_queue[j].floor;
                relevantOrders[relIndex].orderType = g_queue[j].orderType;
                relIndex++;
            }
            else if(g_queue[j].orderType == HARDWARE_ORDER_UP){
                upOrders[upIndex].floor = g_queue[j].floor;
                upOrders[upIndex].orderType = g_queue[j].orderType;
                upIndex++;
            }
        }
        //bubblesort
        int swapped, count;
        ElevatorOrder temp;
        while(1){
            if (OrdersDown+OrdersInside==0){
                break;
            }
            swapped=0;
            for (count=0; count<OrdersDown+OrdersInside-1;count++){
                if (relevantOrders[count].floor < relevantOrders[count+1].floor){
                    temp.floor=relevantOrders[count].floor;
                    temp.orderType=relevantOrders[count].orderType;

                    relevantOrders[count].floor=relevantOrders[count+1].floor;
                    relevantOrders[count].orderType=relevantOrders[count+1].orderType;

                    relevantOrders[count+1].floor=temp.floor;
                    relevantOrders[count+1].orderType=temp.orderType;

                    swapped=1;
                }
            }
            if (swapped==0){
                break;
            }
        }
        int queueIndex=0;
        int fromIndex=0;
        for (int i =0; i<OrdersDown+OrdersInside; i++){
            if (relevantOrders[i].floor<=g_floor){
                fromIndex=i;
                break;
            }
        }
        for (int i =fromIndex; i<OrdersDown+OrdersInside; i++){
            g_queue[queueIndex].floor=relevantOrders[i].floor;
            g_queue[queueIndex].orderType=relevantOrders[i].orderType;
            queueIndex++;
        }
        
        for(int i=0; i<fromIndex; i++){
            g_queue[queueIndex].floor=relevantOrders[i].floor;
            g_queue[queueIndex].orderType=relevantOrders[i].orderType;
            queueIndex++;

        }
        for(int i=0; i<upIndex; i++){
            g_queue[queueIndex].floor=upOrders[i].floor;
            g_queue[queueIndex].orderType=upOrders[i].orderType;
            queueIndex++;
        }

        for (int i = 0; i < g_queue_length-OrdersInside-OrdersUp-OrdersDown; i++){
            g_queue[queueIndex].floor     = -1;
            g_queue[queueIndex].orderType = HARDWARE_ORDER_INSIDE;
            queueIndex++;
        }


        free(relevantOrders);
        free(upOrders);
    }
        break;
    default:
        break;
    }
}