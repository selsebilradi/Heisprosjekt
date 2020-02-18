/**
 * @file
 * @brief Library for functions for modifying a queue of ElevatorOrders
 */
#ifndef QUEUE_H
#define QUEUE_H

#include "hardware.h"
#include "elevatorStateMachine.h"
typedef struct{
	double floor;
	HardwareOrder orderType;
}ElevatorOrder;

 /**
 * @brief Pops the first element of @p queue
 *
 * @param queue A queue of ElevatorOrders
 * @param length Length of @p queue 
 */
void popQueue(ElevatorOrder* queue, int length);


 /**
 * @brief Delete all orders to floor @p floor
 *
 * @param queue A queue of ElevatorOrders
 * @param length Length of @p queue 
 * @param floor Floor number of the floor that all orders to shall be deleted
 */

void deleteOrdersOnFloor(ElevatorOrder * queue, int length, double floor);

 /**
 * @brief Sorts @p queue for efficient and correct elevator behaviourss
 *
 * @param queue A queue of ElevatorOrders
 * @param length Length of @p queue 
 * @param state State of elevator
 */
void sortQueue(ElevatorOrder* queue, int length, State state);

/**
 * @brief Adds order to the queue if the order is not placed in queue before.
 *
 * @param floor The desired floor for an order.
 * @param orderType The type of order.
 * 
 */
void addOrder(int floor,HardwareOrder orderType);


#endif
