/**
 * @file
 * @brief Elevator functionality
 *
 * 
 */
#include <stdio.h>
#ifndef ELEVATORSTATEMACHINE_H

/**
 * @brief Defines states used in ElevatorFSM
 */
typedef enum {
	MOVE_UP,
	MOVE_DOWN, 
	STANDING_STILL, 
	DOOR_OPEN
} state;

/**
 * @brief Functionality for when desired floor is reached
 */
void destination_reached(double floor);

/**
 * @brief Checks if queue is empty.
 * 
 * @return Returns 0 if queue is empty. Returns 1 otherwise.
 */
int checkQueue(elevatorOrder * queue);

/**
 * @brief Clears the queue for all orders and fills it with empty orders.
 * 
 * @warning Will only be called when stop button is pushed.
 */
void clearQueue(elevatorOrder * queue);

/**
 * @brief Sets the elevator to an undefined state and drives it to an defined state before start.
 * 
 */

void elevator_init();

/**
 * @brief Elevator functionality when stop button is pressed.
 * 
 */
void elevator_safety_function(); //kan ha et bedre navn
/**
 * @brief Changes the state of the elevator when queue is empty.
 * 
 */
void empty_queue();

#endif

