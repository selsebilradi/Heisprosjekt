/**
 * @file
 * @brief Elevator functionality
 *
 * 
 */
#include <stdio.h>
#ifndef ELEVATORSTATEMACHINE_H

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
 *
 */

void elevator_init();

#endif

