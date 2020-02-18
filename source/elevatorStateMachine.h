
/**
 * @file
 * @brief Elevator functionality
 *
 * 
 */
#include <stdio.h>
#include "queue.h"
#ifndef ELEVATORSTATEMACHINE_H

/**
 * @brief Defines states used in ElevatorFSM
 */
typedef enum {
	MOVE_UP,
	MOVE_DOWN, 
	STANDING_STILL, 
	DOOR_OPEN
} State;

/**
 * @brief Functionality for when desired floor is reached
 * 
 * @param floor The desired floor the elevator has reached
 */
void floorReached(int floor);

/**
 * @brief Checks if queue is empty.
 * 
 * @param queue A queue of ElevatorOrder
 * 
 * @return Returns 0 if queue is empty. Returns 1 otherwise.
 */
int checkQueue(ElevatorOrder * queue);

/**
 * @brief Clears the queue for all orders and fills it with empty orders.
 * 
 * @param queue A queue of ElevatorOrder
 * 
 * @warning Will only be called when stop button is pushed.
 */
void clearQueue(ElevatorOrder * queue);

/**
 * @brief Sets the elevator to floor 1 as default start.
 * 
 */

void elevatorInit();

/**
 * @brief Elevator functionality when stop button is pressed.
 * 
 */
void elevatorSafetyFunction(); //kan ha et bedre navn
/**
 * @brief Changes the state of the elevator when queue is empty.
 * 
 */
void emptyQueue();

/**
 * @brief Countdown for lights and door.
 * 
 */
void timer(int seconds);

/**
 * @brief Checks if elevator is at floor in the first order.
 * 
 * @param seconds The desired amout of seconds we wish to stall the elevator.
 */
int destinationIsReached();

/**
 * @brief Sets the order lights and floor indicator lights based on where the elevator is.
 * 
 */
void lights();

/**
 * @brief The logic for the elevator.
 * 
 * @return Returns 1 if elevator is at ordered floor, and 0 if not.
 */
int main();


#endif

