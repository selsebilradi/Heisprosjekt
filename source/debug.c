#include "hardware.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int floor;
	HardwareOrder orderType;
} ElevatorOrder;

typedef enum {
	MOVE_UP,
	MOVE_DOWN, 
	STANDING_STILL, 
	DOOR_OPEN
} State;


ElevatorOrder g_queue[10];
int g_queue_length = 10;
State g_state;
int g_floor;

void printQueue2(ElevatorOrder* queue, int length){
		printf("\n{ ");

	for (int i = 0; i < length; i++){
		printf("{ %d, ", queue[i].floor);
		if (queue[i].orderType == HARDWARE_ORDER_INSIDE){
			printf("INSIDE");
		}
		else if (queue[i].orderType == HARDWARE_ORDER_UP){
			printf("UP");
		}
		else if (queue[i].orderType == HARDWARE_ORDER_DOWN){
			printf("DOWN");
		}

		if (i != length - 1){
		printf(" }, ");
		}
		else{
			printf(" }");
		}
	}
	printf(" }\n\n");
}

void addOrder(int floor,HardwareOrder orderType){
	int duplicate=0;
	for (int i=0; i<g_queue_length; i++){
		if (g_queue[i].floor==floor && g_queue[i].orderType==orderType){
			duplicate=1;	
		}
		
	}
	
	if (duplicate==0){
		for (int j=0; j<g_queue_length; j++){
				if(g_queue[j].floor==-1){
					g_queue[j].floor=floor;
					g_queue[j].orderType=orderType;
					break;
		}
	
	}
  }
}  
void clearQueue(ElevatorOrder * queue, int length){
	for (int i = 0; i < length; i++){
		queue[i].floor=-1;
		queue[i].orderType=HARDWARE_ORDER_INSIDE;
	}
}

void sortQueue(){
    switch (g_state)
    {
    case MOVE_UP:
        for (int i=0; i<g_queue_length;i++){
            if (g_queue[i].orderType!=HARDWARE_ORDER_DOWN)
                if (g_floor<=g_queue[i].floor && g_floor<g_queue[i+1].floor){
                    if (g_queue[i].floor>g_queue[i+1].floor){
                        //bubblesort
                        ElevatorOrder temp;
                        temp.floor=g_queue[i+1].floor;
                        temp.orderType=g_queue[i+1].orderType;
                        g_queue[i+1].floor=g_queue[i].floor;
                        g_queue[i+1].orderType=g_queue[i].orderType;
                        g_queue[i].floor=temp.floor;
                        g_queue[i].orderType=temp.orderType;
                    }
                }
            }
        break;
    case MOVE_DOWN:
        for (int j=0; j<g_queue_length;j++){
            if (g_queue[j].orderType!=HARDWARE_ORDER_UP){
                if (g_floor>g_queue[j].floor){
                    if (g_queue[j].floor<g_queue[j+1].floor){
                        //bubblesort
                        ElevatorOrder temp;
                        temp.floor=g_queue[j].floor;
                        temp.orderType=g_queue[j].orderType;
                        g_queue[j].floor=g_queue[j+1].floor;
                        g_queue[j].orderType=g_queue[j+1].orderType;
                        g_queue[j+1].floor=temp.floor;
                        g_queue[j+1].orderType=temp.orderType;
                    }    
                }
            }
        }

        break;
    default:
        break;
    }
}

void sortQueue2(){
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


void printQueue(){
		printf("\n{ ");

	for (int i = 0; i < g_queue_length; i++){
		printf("{ %d, ", g_queue[i].floor);
	
		if (g_queue[i].orderType == HARDWARE_ORDER_INSIDE){
			printf("INSIDE");
		}
		else if (g_queue[i].orderType == HARDWARE_ORDER_UP){
			printf("UP");
		}
		else if (g_queue[i].orderType == HARDWARE_ORDER_DOWN){
			printf("DOWN");
		}

		if (i != g_queue_length - 1){
		printf(" }, ");
		}
		else{
			printf(" }");
		}
	}
	printf(" }\n\n");
}
    

int main(){
    int testFloor;
    int testFloor2;
    HardwareOrder testOrder;
    printf("Enter a floor: ");
    scanf("%d", &testFloor);
    printf("Enter another floor: ");
    scanf("%d", &testFloor2);
    clearQueue(g_queue, g_queue_length);
    addOrder(testFloor, HARDWARE_ORDER_UP);
    addOrder(testFloor2, HARDWARE_ORDER_UP);
    addOrder(0, HARDWARE_ORDER_INSIDE);
    addOrder(3, HARDWARE_ORDER_INSIDE);
    addOrder(1, HARDWARE_ORDER_INSIDE);
    addOrder(2, HARDWARE_ORDER_UP);
    addOrder(2, HARDWARE_ORDER_DOWN);
    addOrder(3, HARDWARE_ORDER_DOWN);
    
    g_state=MOVE_DOWN;
    g_floor=1;
    printQueue();
    sortQueue2();
    

    //printf("Enter an order: ");
    //scanf("", &testOrder);  
    //for (int i=0; i<g_queue_length; i++){
       // printf("%d\n",g_queue[i].floor);
    //}
    printQueue();
    


    return 0;
}