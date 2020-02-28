/**
 * @file
 * @brief Library for functions for modifying a queue of ElevatorOrders
 */
#ifndef QUEUE_H
#define QUEUE_H

#include "hardware.h"


typedef struct{
	int floor;
	HardwareOrder orderType;
} ElevatorOrder;


 /**
 * @brief Delete all orders to floor @p floor
 *
 * @param queue A queue of ElevatorOrders
 * @param length Length of @p queue 
 * @param floor Floor number of the floor that all orders to shall be deleted
 */

void deleteOrdersOnFloor(ElevatorOrder * queue, int length, int floor);


/**
 * @brief Adds order to the queue if the order is not placed in queue before.
 *
 * @param floor The desired floor for an order.
 * @param orderType The type of order.
 * 
 */
void addOrder(int floor,HardwareOrder orderType);

/**
 * @brief Sorts the queue for optimal order execution.
 *
 * 
 */
void sortQueue();

/**
 * @brief Checks if queue is empty.
 * 
 * @param queue A queue of ElevatorOrder
 * 
 * @return Returns 0 if queue is empty. Returns 1 otherwise.
 */
int checkQueue();

/**
 * @brief Clears the queue for all orders and fills it with empty orders.
 * 
 * @param queue A queue of ElevatorOrder
 * @param length Length of queue
 * 
 * @warning Will only be called when stop button is pushed.
 */
void clearQueue();


#endif
