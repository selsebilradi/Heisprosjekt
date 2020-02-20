#include "hardware.h"
#include <stdio.h>

typedef struct{
	int floor;
	HardwareOrder orderType;
} ElevatorOrder;

ElevatorOrder g_queue[6];
int g_queue_length = 6;


void addOrder(int floor,HardwareOrder orderType){
	int duplicate=0;
	for (int i=0; i<g_queue_length; i++){
		if (g_queue[i].floor==floor && g_queue[i].orderType== orderType){
			duplicate=1;	
		}
		
	}
	
	if (duplicate==0){
		for (int j=0; j<g_queue_length; j++){
				if(g_queue[j].floor==-1 && g_queue[j].orderType==NONE){
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
		queue[i].orderType=NONE;
	}
}

void sortQueue(ElevatorOrder * queue, State state, int floor){
    switch (g_state)
    {
    case MOVE_UP:
        for (int i=0; i<g_queue_length;i++){
            if (g_queue[i].orderType==HARDWARE_MOVEMENT_UP){
                if (floor<g_queue[i].floor){
                    if (g_queue[i].floor<g_queue[i+1].floor){
                        ElevatorOrder temp;
                        temp.floor=g_queue[i].floor;
                        temp.orderType=g_queue[i].orderType;
                        g_queue[i].floor=g_queue[i+1].floor;
                        g_queue[i].orderType=g_queue[i+1].orderType;
                        g_queue[i+1].floor=temp.floor;
                        g_queue[i+1].orderType=temp.orderType;

                    }
                }
            }
        }
        
        break;
    case MOVE_DOWN
        for (int j=0; j<g_queue_length;j++){
            if (g_queue[j].orderType==HARDWARE_MOVEMENT_DOWN){
                if (floor>g_queue[j].floor){
                    if (g_queue[j].floor<g_queue[j+1].floor){
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
    

int main(){
    int testFloor;
    int testFloor2;
    HardwareOrder testOrder;
    printf("Enter a floor: ");
    scanf("%d", &testFloor);
    printf("Enter another floor: ");
    scanf("%d", &testFloor2);
    clearQueue(g_queue, g_queue_length);
    addOrder(testFloor, HARDWARE_ORDER_DOWN);
    addOrder(testFloor2, HARDWARE_ORDER_UP);
    addOrder(3, HARDWARE_ORDER_INSIDE);
    

    //printf("Enter an order: ");
    //scanf("", &testOrder);  
    for (int i=0; i<g_queue_length; i++){
        printf("%d\n",g_queue[i].floor);
    }



    return 0;
}